# Logging Module Usage

## Overview
The logging module provides flexible debugging capabilities with multiple log levels and formatting options.

## Features
- Multiple log levels: ERROR, WARN, INFO, DEBUG, TRACE
- Timestamp with microsecond precision
- File, line, and function information
- Hexdump utility for binary data
- Optional ANSI color output
- Configurable at runtime

## Log Levels

| Level | Value | Description |
|-------|-------|-------------|
| `LOG_LEVEL_NONE` | 0 | No logging |
| `LOG_LEVEL_ERROR` | 1 | Critical errors only |
| `LOG_LEVEL_WARN` | 2 | Warnings and errors |
| `LOG_LEVEL_INFO` | 3 | General information (default) |
| `LOG_LEVEL_DEBUG` | 4 | Detailed debugging info |
| `LOG_LEVEL_TRACE` | 5 | Very verbose tracing |

## Basic Usage

### Initialize Logging
```c
#include "log.h"

int main() {
    log_init();  // Initializes stdio and sets default level to INFO
    
    // Your code here
}
```

### Set Log Level
```c
// Set at startup
log_set_level(LOG_LEVEL_DEBUG);

// Or change at runtime
log_set_level(LOG_LEVEL_ERROR);  // Only show errors
```

### Using Log Macros

#### Detailed Logging (with file/line/function info)
```c
LOG_ERROR("Failed to initialize device: error code %d", error_code);
LOG_WARN("Temperature high: %d°C", temp);
LOG_INFO("Device initialized successfully");
LOG_DEBUG("Register value: 0x%04X", reg_val);
LOG_TRACE("Entering function with param: %d", param);
```

Output example:
```
[    5.123] [ERROR] ili9225.c:55 ili9225_init() - Failed to initialize device: error code -1
[    5.124] [ WARN] ili9225.c:78 check_temp() - Temperature high: 85°C
[    5.125] [ INFO] ili9225.c:99 ili9225_init() - Device initialized successfully
```

#### Simple Logging (cleaner output)
```c
LOG_INFO_SIMPLE("System ready");
LOG_DEBUG_SIMPLE("Processing data: %d bytes", len);
```

Output example:
```
[INFO]  System ready
[DEBUG] Processing data: 256 bytes
```

### Hexdump Utility
```c
uint8_t buffer[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

log_hexdump(LOG_LEVEL_DEBUG, "SPI Buffer", buffer, 16);
```

Output:
```
[DEBUG] SPI Buffer (16 bytes):
  0000: 00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f |................|
```

## Conditional Logging

### Compile-Time Debug Logging
```c
// In your source file
#define ILI9225_DEBUG_LOGGING

#ifdef ILI9225_DEBUG_LOGGING
    LOG_DEBUG("Detailed debug info that's only compiled in debug builds");
#endif
```

### Runtime Control
```c
// Disable all logging
log_set_level(LOG_LEVEL_NONE);

// Enable only critical errors
log_set_level(LOG_LEVEL_ERROR);

// Enable verbose debug output
log_set_level(LOG_LEVEL_DEBUG);
```

## Color Output (Optional)

To enable ANSI color output, add to your CMakeLists.txt:
```cmake
target_compile_definitions(ili9225_pico PRIVATE LOG_USE_COLOR)
```

Colors:
- ERROR: Bold Red
- WARN: Bold Yellow
- INFO: Bold Green
- DEBUG: Bold Cyan
- TRACE: Bold White

## Performance Considerations

1. **Log Level Check**: Messages above the current log level are filtered out with minimal overhead
2. **Conditional Compilation**: Use `#ifdef` for debug-only logging to completely eliminate it in release builds
3. **String Formatting**: Only formatted when the log level allows it (lazy evaluation)

## Best Practices

### Use Appropriate Levels
```c
// ✅ Good
LOG_ERROR("Device initialization failed");  // Critical issues
LOG_WARN("Buffer 90% full");               // Potential issues
LOG_INFO("Device connected");              // Important events
LOG_DEBUG("Register 0x10 = 0x1234");      // Debugging info
LOG_TRACE("Pixel (%d, %d) drawn", x, y);  // Verbose details

// ❌ Avoid
LOG_ERROR("Drawing pixel");                // Not an error
LOG_TRACE("Device failed");                // Should be ERROR
```

### Production Deployment
```c
// For production, set a conservative log level
#ifdef RELEASE_BUILD
    log_set_level(LOG_LEVEL_ERROR);
#else
    log_set_level(LOG_LEVEL_DEBUG);
#endif
```

### Debugging Specific Modules
```c
// Enable detailed logging only for the module you're debugging
void my_module_init(void) {
    log_level_t old_level = log_get_level();
    log_set_level(LOG_LEVEL_TRACE);  // Enable all logs
    
    // Your debugging code here
    
    log_set_level(old_level);  // Restore previous level
}
```

## Example: ILI9225 Driver Logging

```c
void ili9225_init(ili9225_config_t* config, ...) {
    if (!config) {
        LOG_ERROR("Config is NULL");
        return;
    }
    
    LOG_INFO("Initializing ILI9225 LCD: %dx%d", width, height);
    LOG_DEBUG("SPI pins - SCK:%d MOSI:%d MISO:%d", pin_sck, pin_mosi, pin_miso);
    
    // ... initialization code ...
    
    LOG_INFO("ILI9225 initialization complete");
}

void ili9225_draw_pixel(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t color) {
    if (x >= config->width || y >= config->height) {
        LOG_WARN("Pixel (%d, %d) out of bounds", x, y);
        return;
    }
    
    LOG_TRACE("Drawing pixel at (%d, %d) with color 0x%04X", x, y, color);
    // ... drawing code ...
}
```

## Disabling Logging for Performance

If you need maximum performance, you can disable logging completely:

```c
// Set to NONE at startup
log_set_level(LOG_LEVEL_NONE);
```

Or use compile-time defines in CMakeLists.txt:
```cmake
# Completely remove logging code in release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(ili9225_pico PRIVATE LOG_LEVEL_NONE)
endif()
```
