# ILI9225 LCD Driver for Raspberry Pi Pico

A lightweight and efficient driver library for ILI9225-based TFT LCD displays (176×220 resolution) for Raspberry Pi Pico/Pico W.

## Branch Structure

- **`release`** - Production-ready code for use in your projects (recommended for git submodules)
- **`main`** - Development branch with testing tools, simulators, and experimental features

## Quick Start

### Using as Git Submodule (Recommended)

Add this library to your Pico project:

```bash
# In your project root
git submodule add -b release https://github.com/latrung124/raspberry-pico-w-ili9225-c-library.git lib/ili9225
git submodule update --init --recursive
```

### CMakeLists.txt Integration

```cmake
# Add the library subdirectory
add_subdirectory(lib/ili9225)

# Link to your executable
target_link_libraries(your_project_name
    ili9225_pico
    pico_stdlib
    hardware_spi
    hardware_gpio
)

# Include directories
target_include_directories(your_project_name PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ili9225/src
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ili9225/src/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ili9225/src/utilities
)
```

## Basic Usage

```c
#include "ili9225.h"
#include "log.h"

int main() {
    // Initialize logging (optional)
    log_init();
    log_set_level(LOG_LEVEL_INFO);
    
    // Configure LCD
    ili9225_config_t lcd;
    ili9225_init(&lcd, 
        spi0,           // SPI instance
        2, 3, 4,        // SCK, MOSI, MISO pins
        5, 6, 7,        // CS, DC, RST pins
        176, 220,       // Width, Height
        ILI9225_PORTRAIT
    );
    
    // Draw something
    ili9225_fill_screen(&lcd, COLOR_BLACK);
    ili9225_draw_text(&lcd, 10, 10, "Hello Pico!", &font_6x8, COLOR_WHITE);
    ili9225_fill_circle(&lcd, 88, 110, 30, COLOR_RED);
    
    while (1) {
        tight_loop_contents();
    }
    
    return 0;
}
```

## Features

- ✅ Basic drawing primitives (pixels, lines, rectangles, circles)
- ✅ Filled shapes support
- ✅ Text rendering with multiple fonts
- ✅ Display rotation/orientation control
- ✅ RGB565 color support
- ✅ Optimized SPI communication
- ✅ Built-in logging system
- ✅ Hardware abstraction layer

## API Overview

### Initialization
- `ili9225_init()` - Initialize display with configuration
- `ili9225_set_orientation()` - Change display orientation

### Drawing Functions
- `ili9225_fill_screen()` - Fill entire screen
- `ili9225_draw_pixel()` - Draw single pixel
- `ili9225_draw_line()` - Draw line
- `ili9225_draw_rect()` - Draw rectangle outline
- `ili9225_fill_rect()` - Draw filled rectangle
- `ili9225_draw_circle()` - Draw circle outline
- `ili9225_fill_circle()` - Draw filled circle
- `ili9225_draw_char()` - Draw single character
- `ili9225_draw_text()` - Draw text string
- `ili9225_draw_bitmap()` - Draw bitmap image

### Common Colors (RGB565)
```c
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
```

## Display Orientations

```c
typedef enum {
    ILI9225_PORTRAIT,              // 0°
    ILI9225_LANDSCAPE,             // 90°
    ILI9225_PORTRAIT_REV,          // 180°
    ILI9225_LANDSCAPE_REV,         // 270°
    ILI9225_PORTRAIT_MIRROR,       // 0° mirrored
    ILI9225_LANDSCAPE_MIRROR,      // 90° mirrored
    ILI9225_PORTRAIT_REV_MIRROR,   // 180° mirrored
    ILI9225_LANDSCAPE_REV_MIRROR   // 270° mirrored
} ili9225_rotation_t;
```

## Hardware Connections

| LCD Pin | Pico Pin | Function |
|---------|----------|----------|
| VCC     | 3.3V     | Power    |
| GND     | GND      | Ground   |
| SCK     | GPIO 2   | SPI CLK  |
| SDA     | GPIO 3   | SPI MOSI |
| RS/DC   | GPIO 6   | Data/Cmd |
| RST     | GPIO 7   | Reset    |
| CS      | GPIO 5   | Chip Sel |

*Note: Pin assignments are configurable in `ili9225_init()`*

## Logging System

The library includes an optional logging system for debugging:

```c
log_init();
log_set_level(LOG_LEVEL_DEBUG);  // NONE, ERROR, WARN, INFO, DEBUG, TRACE

LOG_INFO("Display initialized");
LOG_DEBUG("Drawing at (%d, %d)", x, y);
```

## Development Branch

Want to contribute or test new features? Check out the `main` branch:

```bash
git clone https://github.com/latrung124/raspberry-pico-w-ili9225-c-library.git
cd raspberry-pico-w-ili9225-c-library
git checkout main
```

The `main` branch includes:
- 🔧 Web-based LCD simulator for testing without hardware
- 🧪 Comprehensive test suites
- 📊 Example projects
- 🛠️ Development tools and mock hardware layer

## Requirements

- Raspberry Pi Pico SDK (2.0.0+)
- CMake (3.13+)
- GCC ARM toolchain
- ILI9225-based LCD display (176×220)

## Examples

See the `examples/` directory for complete working examples:
- `example_with_logging.c` - Basic usage with logging
- More examples in development branch

## License

Copyright (c) 2025 trung.la

See LICENSE file for details.

## Support

- 🐛 Issues: [GitHub Issues](https://github.com/latrung124/raspberry-pico-w-ili9225-c-library/issues)
- 📖 Documentation: See header files in `src/`
- 💡 Examples: Check `examples/` directory

## Contributing

Contributions are welcome! Please work on the `main` branch for new features and submit pull requests.

1. Fork the repository
2. Create a feature branch from `main`
3. Test your changes with the simulator
4. Submit a pull request

---

**Quick Submodule Commands:**

```bash
# Clone project with submodules
git clone --recursive <your-project-url>

# Update submodules to latest release
git submodule update --remote --merge

# Remove submodule (if needed)
git submodule deinit -f lib/ili9225
git rm -f lib/ili9225
rm -rf .git/modules/lib/ili9225
```
