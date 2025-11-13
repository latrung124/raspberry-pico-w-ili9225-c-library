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
    static uint dc_pin = 0;
    static uint cs_pin = 0;
    static bool pins_identified = false;
    
    if (!pins_identified) {
        // Heuristic: DC and CS are typically different pins used frequently together
        if (dc_pin == 0) dc_pin = pin;
        else if (cs_pin == 0 && pin != dc_pin) {
            cs_pin = pin;
            sim_state.pin_dc = dc_pin;
            sim_state.pin_cs = cs_pin;
            pins_identified = true;
            printf("[MOCK] Identified pins - DC:%d CS:%d\n", dc_pin, cs_pin);
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
        
        if (last_command == ILI9225_GRAM_DATA_REG) {
            sim_state.in_gram_write = true;
            printf("[MOCK] Starting GRAM write at (%d,%d)\n", 
                   sim_state.current_x, sim_state.current_y);
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
    if (!sim_state.connected || !sim_state.fb_dirty) {
        return;
    }
    
    init_framebuffer();
    
    printf("[MOCK] Flushing framebuffer to simulator...\n");
    
    // Send each pixel that's been written
    for (uint16_t y = 0; y < sim_state.fb_height; y++) {
        for (uint16_t x = 0; x < sim_state.fb_width; x++) {
            uint16_t color = sim_state.framebuffer[y * sim_state.fb_width + x];
            if (color != 0) {  // Only send non-black pixels (optimization)
                char cmd[128];
                snprintf(cmd, sizeof(cmd), 
                        "{\"type\":\"pixel\",\"x\":%u,\"y\":%u,\"color\":%u}\n",
                        x, y, color);
                send_sim_command(cmd);
            }
        }
    }
    
    sim_state.fb_dirty = false;
    printf("[MOCK] Framebuffer flush complete\n");
}
