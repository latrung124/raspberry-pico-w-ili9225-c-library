# Logging Module

A lightweight, flexible logging system for Raspberry Pi Pico projects with multiple log levels, timestamps, and hexdump capabilities.

## Quick Start

```c
#include "log.h"

int main() {
    log_init();                      // Initialize logging
    log_set_level(LOG_LEVEL_DEBUG);  // Set desired level
    
    LOG_INFO("Hello from Pico!");
    LOG_DEBUG("Value: %d", 42);
}
```

## Features

✅ **5 Log Levels**: ERROR, WARN, INFO, DEBUG, TRACE  
✅ **Timestamps**: Microsecond-precision timing  
✅ **Context Info**: File, line, and function name  
✅ **Hexdump**: Binary data visualization  
✅ **Runtime Control**: Change log level on-the-fly  
✅ **Zero Overhead**: Disabled logs have minimal performance impact  
✅ **Optional Colors**: ANSI color support (define `LOG_USE_COLOR`)  

## Files

- `log.h` - Header file with macros and function declarations
- `log.c` - Implementation
- `LOG_USAGE.md` - Detailed usage documentation
- `examples/example_with_logging.c` - Complete example

## Log Levels

| Level | Macro | Use Case |
|-------|-------|----------|
| ERROR | `LOG_ERROR()` | Critical failures |
| WARN  | `LOG_WARN()` | Potential issues |
| INFO  | `LOG_INFO()` | Important events |
| DEBUG | `LOG_DEBUG()` | Debugging info |
| TRACE | `LOG_TRACE()` | Verbose details |

## API Reference

### Initialization
```c
void log_init(void);                     // Initialize stdio and set default level
void log_set_level(log_level_t level);   // Change log level
log_level_t log_get_level(void);         // Get current level
```

### Logging Macros
```c
LOG_ERROR(fmt, ...);   // Error messages
LOG_WARN(fmt, ...);    // Warnings
LOG_INFO(fmt, ...);    // Information
LOG_DEBUG(fmt, ...);   // Debug info
LOG_TRACE(fmt, ...);   // Trace details
```

### Simple Logging (no file/line)
```c
LOG_ERROR_SIMPLE(fmt, ...);
LOG_INFO_SIMPLE(fmt, ...);
// etc...
```

### Hexdump
```c
void log_hexdump(log_level_t level, const char* label, 
                 const void* data, size_t len);
```

## Example Output

```
[    0.125] [ERROR] ili9225.c:55 ili9225_init() - Failed to initialize: -1
[    0.126] [ WARN] ili9225.c:78 check_status() - Temperature high: 85°C
[    0.127] [ INFO] ili9225.c:99 ili9225_init() - Device ready
[    0.128] [DEBUG] ili9225.c:120 setup_spi() - SPI initialized at 10 MHz
[DEBUG] SPI Buffer (16 bytes):
  0000: 00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f |................|
```

## Configuration

### CMakeLists.txt
```cmake
# Add log.c to your library
add_library(my_lib
    src/main.c
    src/utilities/log.c
)

# Optional: Enable colors
target_compile_definitions(my_lib PRIVATE LOG_USE_COLOR)
```

### Runtime Configuration
```c
// Production: errors only
log_set_level(LOG_LEVEL_ERROR);

// Development: verbose debug
log_set_level(LOG_LEVEL_DEBUG);

// Disable completely
log_set_level(LOG_LEVEL_NONE);
```

## Performance

- **Compile-time filtering**: Messages above current level are not formatted
- **Minimal overhead**: Level check is a simple integer comparison
- **Optional compilation**: Wrap debug logs in `#ifdef` for zero overhead in release builds

```c
#ifdef DEBUG_BUILD
    LOG_DEBUG("This is completely removed in release builds");
#endif
```

## Integration with ILI9225 Driver

The ILI9225 driver includes logging support. Enable detailed logging:

```c
// In ili9225.c, uncomment:
#define ILI9225_DEBUG_LOGGING

// Then in your main:
log_set_level(LOG_LEVEL_DEBUG);
```

## License

Same as parent project (Copyright © 2025 trung.la)

## See Also

- [LOG_USAGE.md](LOG_USAGE.md) - Detailed usage guide
- [example_with_logging.c](../examples/example_with_logging.c) - Complete example
