/**
 * @file log.h
 * @author trung.la
 * @date November 13 2025
 * @brief Logging interface for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Log levels
typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level_t;

// Global log level - can be changed at runtime
extern log_level_t g_log_level;

// Initialize logging (sets up stdio)
void log_init(void);

// Set log level
void log_set_level(log_level_t level);

// Get current log level
log_level_t log_get_level(void);

// Internal logging function
void log_print(log_level_t level, const char* file, int line, const char* func, const char* fmt, ...);

// Logging macros
#define LOG_ERROR(fmt, ...) log_print(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  log_print(LOG_LEVEL_WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  log_print(LOG_LEVEL_INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) log_print(LOG_LEVEL_TRACE, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

// Simplified macros without file/line info (for cleaner output)
#define LOG_ERROR_SIMPLE(fmt, ...) do { if (g_log_level >= LOG_LEVEL_ERROR) printf("[ERROR] " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_WARN_SIMPLE(fmt, ...)  do { if (g_log_level >= LOG_LEVEL_WARN)  printf("[WARN]  " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_INFO_SIMPLE(fmt, ...)  do { if (g_log_level >= LOG_LEVEL_INFO)  printf("[INFO]  " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_DEBUG_SIMPLE(fmt, ...) do { if (g_log_level >= LOG_LEVEL_DEBUG) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOG_TRACE_SIMPLE(fmt, ...) do { if (g_log_level >= LOG_LEVEL_TRACE) printf("[TRACE] " fmt "\n", ##__VA_ARGS__); } while(0)

// Hexdump utility for debugging binary data
void log_hexdump(log_level_t level, const char* label, const void* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif // UTILITIES_LOG_H
