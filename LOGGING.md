# Logging Module - Summary

## What Was Added

A complete logging system for the ILI9225 library with the following components:

### New Files Created
1. **`src/utilities/log.h`** - Logging header with macros and API
2. **`src/utilities/log.c`** - Logging implementation
3. **`src/utilities/README.md`** - Quick reference guide
4. **`src/utilities/LOG_USAGE.md`** - Detailed usage documentation
5. **`examples/example_with_logging.c`** - Complete working example

### Modified Files
1. **`CMakeLists.txt`** - Added log.c to build
2. **`src/ili9225.c`** - Added logging to key functions

## Key Features

### 1. Multiple Log Levels
- `LOG_LEVEL_NONE` (0) - Disable all logging
- `LOG_LEVEL_ERROR` (1) - Critical errors only
- `LOG_LEVEL_WARN` (2) - Warnings and errors
- `LOG_LEVEL_INFO` (3) - General information (default)
- `LOG_LEVEL_DEBUG` (4) - Detailed debugging
- `LOG_LEVEL_TRACE` (5) - Very verbose tracing

### 2. Two Logging Styles

#### Detailed (with context)
```c
LOG_ERROR("Device failed with code %d", -1);
// Output: [    0.125] [ERROR] ili9225.c:55 ili9225_init() - Device failed with code -1
```

#### Simple (clean output)
```c
LOG_INFO_SIMPLE("System ready");
// Output: [INFO]  System ready
```

### 3. Hexdump Utility
```c
log_hexdump(LOG_LEVEL_DEBUG, "Buffer", data, 16);
// Output:
// [DEBUG] Buffer (16 bytes):
//   0000: 00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f |................|
```

### 4. Runtime Configuration
```c
log_init();                      // Initialize
log_set_level(LOG_LEVEL_DEBUG);  // Set level
log_level_t current = log_get_level();  // Query current level
```

### 5. Performance Optimizations
- Compile-time filtering for disabled levels
- Optional `#ifdef` wrapping for zero overhead
- Minimal impact when log level filters messages

## Usage in ILI9225 Driver

### Automatic Logging
The driver now logs key events:

```c
ili9225_init(...)
  // Logs: INFO - "Initializing ILI9225 LCD: 176x220"
  // Logs: DEBUG - Pin configurations
  // Logs: DEBUG - SPI initialization
  // Logs: INFO - "ILI9225 initialization complete"
```

### Conditional Debug Logging
Enable detailed pixel-level logging:

```c
// In ili9225.c, uncomment:
#define ILI9225_DEBUG_LOGGING

// This enables bounds checking logs in draw_pixel
```

## Quick Integration Guide

### In Your Main Application

```c
#include "log.h"
#include "ili9225.h"

int main() {
    // 1. Initialize logging first
    log_init();
    
    // 2. Set desired level
    #ifdef DEBUG
        log_set_level(LOG_LEVEL_DEBUG);
    #else
        log_set_level(LOG_LEVEL_ERROR);
    #endif
    
    // 3. Use logging
    LOG_INFO("Application starting");
    
    // 4. Initialize display (logs automatically)
    ili9225_config_t lcd;
    ili9225_init(&lcd, ...);
    
    // 5. Your application code with logging
    LOG_DEBUG("Drawing shapes");
    ili9225_draw_circle(&lcd, 50, 50, 20, 0xF800);
    
    LOG_INFO("Application ready");
    
    while (1) {
        // Main loop
    }
}
```

## Build Configuration

### Current Setup
Already configured in `CMakeLists.txt`:
```cmake
add_library(ili9225_pico STATIC
    src/ili9225.c
    src/utilities/log.c
)
```

### Optional: Enable Colors
Add to CMakeLists.txt:
```cmake
target_compile_definitions(ili9225_pico PRIVATE LOG_USE_COLOR)
```

### Optional: Release Build Configuration
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(ili9225_pico PRIVATE LOG_LEVEL_ERROR)
endif()
```

## Examples Output

### Normal Operation
```
[    0.001] [ INFO] ili9225.c:56 ili9225_init() - Initializing ILI9225 LCD: 176x220
[    0.002] [DEBUG] ili9225.c:57 ili9225_init() - SPI pins - SCK:2 MOSI:3 MISO:4
[    0.003] [DEBUG] ili9225.c:58 ili9225_init() - Control pins - CS:5 DC:6 RST:7
[    0.015] [DEBUG] ili9225.c:93 ili9225_init() - SPI initialized at 10 MHz
[    0.016] [DEBUG] ili9225.c:96 ili9225_init() - Performing hardware reset
[    0.137] [DEBUG] ili9225.c:103 ili9225_init() - Starting ILI9225 register initialization
[    0.245] [ INFO] ili9225.c:177 ili9225_init() - ILI9225 initialization complete
```

### Error Condition
```
[    0.001] [ERROR] ili9225.c:54 ili9225_init() - Config is NULL
```

### Debug Mode with Bounds Checking
```
[    5.123] [TRACE] ili9225.c:217 ili9225_draw_pixel() - draw_pixel: out of bounds (200, 250) - display is 176x220
```

## Documentation

- **README.md** - This file (quick reference)
- **LOG_USAGE.md** - Comprehensive usage guide with examples
- **example_with_logging.c** - Complete working example

## Testing

Build verified successful:
```bash
cd build && cmake --build .
# Result: [100%] Built target ili9225_pico
```

## Future Enhancements

Potential improvements:
- [ ] Log to SD card
- [ ] Network logging (if using WiFi)
- [ ] Circular buffer for crash dumps
- [ ] Custom output handlers
- [ ] Per-module log levels

## License

Copyright (c) 2025 trung.la
Part of the ILI9225 Pico library
