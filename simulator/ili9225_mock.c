/**
 * @file ili9225_mock.c
 * @author trung.la
 * @date November 13 2025
 * @brief Mock implementation that captures SPI/GPIO and converts to simulator commands
 * 
 * Copyright (c) 2025 trung.la
 */

#include "ili9225_mock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

// Mock SPI instances
spi_inst_t spi0_inst = {0};
spi_inst_t spi1_inst = {1};

// State tracking
static struct {
    int socket_fd;
    bool connected;
    uint8_t pin_dc_state;
    uint8_t pin_cs_state;
    bool is_command;
    
    // Track current drawing state
    uint16_t current_x;
    uint16_t current_y;
    uint16_t window_x1, window_y1, window_x2, window_y2;
    bool in_gram_write;
    
    // Pin mappings (to identify which pins are which)
    uint pin_dc;
    uint pin_cs;
    
    // Buffering for batch operations
    uint16_t* framebuffer;
    uint16_t fb_width;
    uint16_t fb_height;
    bool fb_dirty;
    
} sim_state = {
    .socket_fd = -1,
    .connected = false,
    .pin_dc_state = 1,
    .pin_cs_state = 1,
    .is_command = false,
    .current_x = 0,
    .current_y = 0,
    .window_x1 = 0, .window_y1 = 0, .window_x2 = 175, .window_y2 = 219,
    .in_gram_write = false,
    .pin_dc = 0,
    .pin_cs = 0,
    .framebuffer = NULL,
    .fb_width = 176,
    .fb_height = 220,
    .fb_dirty = false
};

// Command tracking
static uint8_t last_command = 0;
static uint16_t command_data[16];
static int command_data_idx = 0;

// ILI9225 Register definitions (from ili9225_defs.h)
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 0x36
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 0x37
#define ILI9225_VERTICAL_WINDOW_ADDR1   0x38
#define ILI9225_VERTICAL_WINDOW_ADDR2   0x39
#define ILI9225_RAM_ADDR_SET1           0x20
#define ILI9225_RAM_ADDR_SET2           0x21
#define ILI9225_GRAM_DATA_REG           0x22

// Helper to send command to simulator
static void send_sim_command(const char* json) {
    if (sim_state.connected && sim_state.socket_fd >= 0) {
        send(sim_state.socket_fd, json, strlen(json), 0);
    }
}

static bool send_all(int fd, const void* data, size_t len) {
    const uint8_t* bytes = (const uint8_t*)data;
    size_t total_sent = 0;

    while (total_sent < len) {
        ssize_t sent = send(fd, bytes + total_sent, len - total_sent, 0);
        if (sent <= 0) {
            return false;
        }
        total_sent += (size_t)sent;
    }
    return true;
}

static char* base64_encode(const uint8_t* data, size_t input_length, size_t* output_length) {
    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const int mod_table[] = {0, 2, 1};

    size_t encoded_len = 4 * ((input_length + 2) / 3);
    char* encoded_data = malloc(encoded_len + 1);
    if (!encoded_data) {
        return NULL;
    }

    size_t input_index = 0;
    size_t output_index = 0;
    while (input_index < input_length) {
        uint32_t octet_a = input_index < input_length ? data[input_index++] : 0;
        uint32_t octet_b = input_index < input_length ? data[input_index++] : 0;
        uint32_t octet_c = input_index < input_length ? data[input_index++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        encoded_data[output_index++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[output_index++] = encoding_table[(triple >> 12) & 0x3F];
        encoded_data[output_index++] = encoding_table[(triple >> 6) & 0x3F];
        encoded_data[output_index++] = encoding_table[triple & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++) {
        encoded_data[encoded_len - 1 - i] = '=';
    }

    encoded_data[encoded_len] = '\0';
    if (output_length) {
        *output_length = encoded_len;
    }
    return encoded_data;
}

// Initialize framebuffer
static void init_framebuffer(void) {
    if (!sim_state.framebuffer) {
        sim_state.framebuffer = calloc(sim_state.fb_width * sim_state.fb_height, sizeof(uint16_t));
        printf("[MOCK] Framebuffer allocated: %dx%d\n", sim_state.fb_width, sim_state.fb_height);
    }
}

// Mock implementations
void gpio_init(uint pin) {
    printf("[MOCK] GPIO init: pin %d\n", pin);
}

void gpio_set_dir(uint pin, int dir) {
    printf("[MOCK] GPIO set dir: pin %d, dir %d\n", pin, dir);
}

void gpio_put(uint pin, int value) {
    // Track DC and CS pin states
    static uint cs_pin = 0;
    static uint dc_pin = 0;
    static bool pins_identified = false;
    
    if (!pins_identified) {
        // Heuristic: first frequent pin is CS (pull high), second distinct pin is DC
        if (cs_pin == 0) {
            cs_pin = pin;
        } else if (dc_pin == 0 && pin != cs_pin) {
            dc_pin = pin;
            sim_state.pin_cs = cs_pin;
            sim_state.pin_dc = dc_pin;
            pins_identified = true;
            printf("[MOCK] Identified pins - CS:%d DC:%d\n", cs_pin, dc_pin);
        }
    }
    
    if (pin == sim_state.pin_dc) {
        sim_state.pin_dc_state = value;
        sim_state.is_command = (value == 0);
    } else if (pin == sim_state.pin_cs) {
        sim_state.pin_cs_state = value;
    }
}

void gpio_set_function(uint pin, int func) {
    printf("[MOCK] GPIO set function: pin %d, func %d\n", pin, func);
}

int spi_init(spi_inst_t* spi, uint baudrate) {
    (void)spi; // Unused in mock
    printf("[MOCK] SPI init: baudrate %d Hz\n", baudrate);
    return 0;
}

void spi_set_format(spi_inst_t* spi, uint data_bits, uint cpol, uint cpha, uint order) {
    (void)spi; (void)cpol; (void)cpha; (void)order; // Unused in mock
    printf("[MOCK] SPI set format: %d bits\n", data_bits);
}

int spi_write_blocking(spi_inst_t* spi, const uint8_t* src, size_t len) {
    (void)spi; // Unused in mock
    if (!sim_state.connected) return len;
    
    init_framebuffer();
    
    if (sim_state.is_command && len == 1) {
        // Command byte
        last_command = src[0];
        command_data_idx = 0;
        sim_state.in_gram_write = (last_command == ILI9225_GRAM_DATA_REG);

        // Only log GRAM writes occasionally to avoid performance issues
        if (sim_state.in_gram_write) {
            static int gram_write_count = 0;
            if (gram_write_count++ % 1000 == 0) {
                printf("[MOCK] Starting GRAM write at (%d,%d)\n",
                       sim_state.current_x, sim_state.current_y);
            }
        }
    } else if (!sim_state.is_command && len == 2) {
        // Data word (16-bit)
        uint16_t data = (src[0] << 8) | src[1];
        
        if (sim_state.in_gram_write) {
            // This is pixel data
            uint16_t x = sim_state.current_x;
            uint16_t y = sim_state.current_y;
            
            // Store in framebuffer
            if (x < sim_state.fb_width && y < sim_state.fb_height) {
                sim_state.framebuffer[y * sim_state.fb_width + x] = data;
                sim_state.fb_dirty = true;
            }
            
            // Auto-increment position (simplified - assumes left-to-right, top-to-bottom)
            sim_state.current_x++;
            if (sim_state.current_x > sim_state.window_x2) {
                sim_state.current_x = sim_state.window_x1;
                sim_state.current_y++;
            }
        } else {
            // Command parameter
            command_data[command_data_idx++] = data;
            
            // Process certain commands
            switch (last_command) {
                case ILI9225_HORIZONTAL_WINDOW_ADDR1:
                    sim_state.window_x2 = data;
                    break;
                case ILI9225_HORIZONTAL_WINDOW_ADDR2:
                    sim_state.window_x1 = data;
                    break;
                case ILI9225_VERTICAL_WINDOW_ADDR1:
                    sim_state.window_y2 = data;
                    break;
                case ILI9225_VERTICAL_WINDOW_ADDR2:
                    sim_state.window_y1 = data;
                    break;
                case ILI9225_RAM_ADDR_SET1:
                    sim_state.current_x = data;
                    break;
                case ILI9225_RAM_ADDR_SET2:
                    sim_state.current_y = data;
                    break;
            }
        }
    }
    
    return len;
}

void sleep_ms(uint32_t ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

// Simulator bridge functions
void sim_mock_init(const char* host, int port) {
    // Create socket
    sim_state.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sim_state.socket_fd < 0) {
        perror("[MOCK] Socket creation failed");
        return;
    }
    
    // Setup server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Convert host to IP
    struct hostent *he = gethostbyname(host);
    if (he == NULL) {
        fprintf(stderr, "[MOCK] Failed to resolve host: %s\n", host);
        close(sim_state.socket_fd);
        return;
    }
    
    memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);
    
    // Connect to simulator
    if (connect(sim_state.socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[MOCK] Connection to simulator failed");
        close(sim_state.socket_fd);
        return;
    }
    
    sim_state.connected = true;
    printf("[MOCK] Connected to simulator at %s:%d\n", host, port);
    
    // Clear the display
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "{\"type\":\"clear\",\"color\":0}\n");
    send_sim_command(cmd);
}

void sim_mock_close(void) {
    if (sim_state.connected) {
        close(sim_state.socket_fd);
        sim_state.connected = false;
        printf("[MOCK] Disconnected from simulator\n");
    }
    
    if (sim_state.framebuffer) {
        free(sim_state.framebuffer);
        sim_state.framebuffer = NULL;
    }
}

void sim_mock_flush_framebuffer(void) {
    printf("[MOCK] flush called - connected:%d dirty:%d\n", sim_state.connected, sim_state.fb_dirty);
    
    if (!sim_state.connected) {
        printf("[MOCK] Not connected, skipping flush\n");
        return;
    }
    
    if (!sim_state.fb_dirty) {
        printf("[MOCK] Framebuffer not dirty, skipping flush\n");
        return;
    }
    
    init_framebuffer();
    
    size_t raw_size = (size_t)sim_state.fb_width * (size_t)sim_state.fb_height * 2u;
    uint8_t* raw_buffer = malloc(raw_size);
    if (!raw_buffer) {
        fprintf(stderr, "[MOCK] Failed to allocate raw framebuffer buffer\n");
        return;
    }

    size_t raw_index = 0;
    for (uint16_t y = 0; y < sim_state.fb_height; y++) {
        for (uint16_t x = 0; x < sim_state.fb_width; x++) {
            uint16_t color = sim_state.framebuffer[y * sim_state.fb_width + x];
            raw_buffer[raw_index++] = (uint8_t)(color >> 8);
            raw_buffer[raw_index++] = (uint8_t)(color & 0xFF);
        }
    }

    size_t encoded_len = 0;
    char* encoded = base64_encode(raw_buffer, raw_size, &encoded_len);
    free(raw_buffer);

    if (!encoded) {
        fprintf(stderr, "[MOCK] Failed to encode framebuffer to base64\n");
        return;
    }

    char header[256];
    int header_len = snprintf(header, sizeof(header),
                              "{\"type\":\"framebuffer\",\"width\":%u,\"height\":%u,\"pixels\":\"",
                              sim_state.fb_width, sim_state.fb_height);
    if (header_len < 0 || header_len >= (int)sizeof(header)) {
        fprintf(stderr, "[MOCK] Failed to compose framebuffer header\n");
        free(encoded);
        return;
    }

    const char footer[] = "\"}\n";

    printf("[MOCK] Flushing framebuffer to simulator (payload %zu bytes)\n", encoded_len);
    bool ok = send_all(sim_state.socket_fd, header, (size_t)header_len);
    if (ok) {
        ok = send_all(sim_state.socket_fd, encoded, encoded_len);
    }
    if (ok) {
        ok = send_all(sim_state.socket_fd, footer, sizeof(footer) - 1);
    }

    free(encoded);

    if (!ok) {
        fprintf(stderr, "[MOCK] Failed to transmit framebuffer to simulator\n");
        return;
    }

    sim_state.fb_dirty = false;
    printf("[MOCK] Framebuffer flush complete\n");
}
