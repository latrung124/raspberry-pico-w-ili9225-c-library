/**
 * @file log.c
 * @author trung.la
 * @date November 13 2025
 * @brief Logging implementation for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

#include "log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/time.h"

// Global log level - default to INFO
log_level_t g_log_level = LOG_LEVEL_INFO;

// Log level strings
static const char* log_level_strings[] = {
    "NONE",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
    "TRACE"
};

// Log level colors (ANSI escape codes) - optional
#ifdef LOG_USE_COLOR
static const char* log_level_colors[] = {
    "\033[0m",      // NONE - reset
    "\033[1;31m",   // ERROR - bold red
    "\033[1;33m",   // WARN - bold yellow
    "\033[1;32m",   // INFO - bold green
    "\033[1;36m",   // DEBUG - bold cyan
    "\033[1;37m"    // TRACE - bold white
};
static const char* color_reset = "\033[0m";
#endif

void log_init(void) {
    stdio_init_all();
    g_log_level = LOG_LEVEL_INFO;  // Default level
}

void log_set_level(log_level_t level) {
    if (level >= LOG_LEVEL_NONE && level <= LOG_LEVEL_TRACE) {
        g_log_level = level;
    }
}

log_level_t log_get_level(void) {
    return g_log_level;
}

void log_print(log_level_t level, const char* file, int line, const char* func, const char* fmt, ...) {
    if (level > g_log_level) {
        return;
    }

    // Get just the filename without path
    const char* filename = strrchr(file, '/');
    if (filename) {
        filename++;
    } else {
        filename = file;
    }

    // Print timestamp
    uint64_t time_us = time_us_64();
    uint32_t seconds = time_us / 1000000;
    uint32_t millis = (time_us % 1000000) / 1000;

#ifdef LOG_USE_COLOR
    printf("%s[%5u.%03u] [%5s] %s:%d %s() - ",
           log_level_colors[level],
           seconds, millis,
           log_level_strings[level],
           filename, line, func);
#else
    printf("[%5u.%03u] [%5s] %s:%d %s() - ",
           seconds, millis,
           log_level_strings[level],
           filename, line, func);
#endif

    // Print the actual message
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

#ifdef LOG_USE_COLOR
    printf("%s\n", color_reset);
#else
    printf("\n");
#endif
}

void log_hexdump(log_level_t level, const char* label, const void* data, size_t len) {
    if (level > g_log_level || !data || len == 0) {
        return;
    }

    const uint8_t* bytes = (const uint8_t*)data;
    
    printf("[%5s] %s (%zu bytes):\n", log_level_strings[level], label, len);
    
    for (size_t i = 0; i < len; i += 16) {
        // Print offset
        printf("  %04zx: ", i);
        
        // Print hex values
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) {
                printf("%02x ", bytes[i + j]);
            } else {
                printf("   ");
            }
            if (j == 7) printf(" ");
        }
        
        // Print ASCII representation
        printf(" |");
        for (size_t j = 0; j < 16 && i + j < len; j++) {
            uint8_t c = bytes[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        printf("|\n");
    }
}
