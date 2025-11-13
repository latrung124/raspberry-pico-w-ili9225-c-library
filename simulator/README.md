# ILI9225 Web Simulator# ILI9225 Web Simulator# ILI9225 Web Simulator# ILI9225 LCD Simulator



Test your `ili9225.c` drawing functions in a web browser without physical hardware!



## Quick Start GuideTest your `ili9225.c` drawing functions in a web browser without physical hardware!



### Step 1: Start the Web Simulator



Open a terminal and run:## PurposeTest your `ili9225.c` drawing functions in a web browser without physical hardware!A Python-based simulator for the ILI9225 176x220 LCD display. Test your drawing code without physical hardware!



```bash

cd simulator

./run_web.shThis simulator allows you to test the **real** `ili9225.c` implementation by:

```

- Mocking the Raspberry Pi Pico hardware layer (SPI, GPIO)

This starts a Flask server at `http://localhost:5000`. **Open this URL in your browser!**

- Capturing drawing operations at the hardware level## Purpose## Features

### Step 2: Build and Run Tests

- Visualizing results in a web browser

Open **another terminal** and run:

- Testing all drawing primitives: pixels, lines, rectangles, circles, text

```bash

cd simulator



# Using CMake (recommended)**No SPI simulation** - we capture the drawing intent and render directly to the display.This simulator allows you to test the **real** `ili9225.c` implementation by:- ✅ **Web-Based Display**: Works in GitHub Codespaces and remote environments

cmake -B build

cmake --build build

./build/test_real_ili9225

```## Features- Mocking the Raspberry Pi Pico hardware layer (SPI, GPIO)- ✅ **Desktop Display**: Optional Pygame version for local development



**Alternative using Make:**

```bash

cd simulator✅ **Web-Based Display**: Works in GitHub Codespaces and remote environments  - Capturing drawing operations at the hardware level- ✅ **Visual Display**: 176x220 pixel simulation window

make

./test_real_ili9225✅ **Real Code Testing**: Tests your actual `ili9225.c` implementation  

```

✅ **Visual Display**: 176x220 pixel simulation with 3x zoom  - Visualizing results in a web browser- ✅ **Scalable**: Adjust zoom level (default 3x)

### Step 3: Watch the Magic! ✨

✅ **RGB565 Support**: Accurate color conversion  

Go to your browser (`http://localhost:5000`) and watch your **real** `ili9225.c` functions draw:

- Pixels✅ **CMake Build System**: Consistent with main project  - Testing all drawing primitives: pixels, lines, rectangles, circles, text- ✅ **Network Mode**: Receive drawing commands over TCP/WebSocket

- Lines

- Rectangles✅ **All Drawing Functions**:

- Circles

- Text  - `ili9225_draw_pixel()`, `ili9225_draw_line()`- ✅ **Demo Mode**: Built-in demonstration



## What This Tests  - `ili9225_draw_rect()`, `ili9225_fill_rect()`



This simulator tests your **actual production code** from `../src/ili9225.c`:  - `ili9225_draw_circle()`, `ili9225_fill_circle()`**No SPI simulation** - we capture the drawing intent and render directly to the display.- ✅ **RGB565 Support**: Accurate color conversion



✅ `ili9225_init()` - Initialization    - `ili9225_draw_text()`, `ili9225_draw_char()`

✅ `ili9225_fill_screen()` - Screen clearing  

✅ `ili9225_draw_pixel()` - Pixel drawing  - ✅ **Screenshot**: Save current display state

✅ `ili9225_draw_line()` - Line drawing  

✅ `ili9225_draw_rect()` - Rectangle outline  ## Installation

✅ `ili9225_fill_rect()` - Filled rectangle  

✅ `ili9225_draw_circle()` - Circle outline  ## Features- ✅ **All Drawing Primitives**:

✅ `ili9225_fill_circle()` - Filled circle  

✅ `ili9225_draw_text()` - Text rendering  ```bash

✅ `ili9225_draw_char()` - Character drawing  

cd simulator  - Pixels

## Installation

pip install -r requirements-web.txt

```bash

cd simulator```✅ **Web-Based Display**: Works in GitHub Codespaces and remote environments    - Lines

pip install -r requirements-web.txt

```



## How It Works## Quick Start✅ **Real Code Testing**: Tests your actual `ili9225.c` implementation    - Rectangles (outline and filled)



```

Your Test Program → Real ili9225.c → Mock Hardware → Framebuffer → Web Browser

```### 1. Start Web Simulator✅ **Visual Display**: 176x220 pixel simulation with 3x zoom    - Circles (outline and filled)



1. **Mock Layer** (`ili9225_mock.c`): Intercepts GPIO/SPI calls

2. **Real Code**: Your actual `ili9225.c` executes normally

3. **Framebuffer**: Captures pixel operations```bash✅ **RGB565 Support**: Accurate color conversion    - Text rendering

4. **WebSocket**: Sends to web browser

5. **Browser**: Displays the LCD screen./run_web.sh



**No SPI simulation** - we capture drawing at the pixel level!```✅ **All Drawing Functions**:



## Writing Your Own Tests



Edit `test_real_ili9225.c`:This starts a Flask server at `http://localhost:5000`. Open the URL in your browser.  - `ili9225_draw_pixel()`, `ili9225_draw_line()`## Installation



```c

void my_custom_test(ili9225_config_t* lcd) {

    // Clear screen to white### 2. Build and Run Test Program  - `ili9225_draw_rect()`, `ili9225_fill_rect()`

    ili9225_fill_screen(lcd, 0xFFFF);

    sim_mock_flush_framebuffer();  // Send to browser

    sleep(1);

    In another terminal:  - `ili9225_draw_circle()`, `ili9225_fill_circle()`### Web Version (Recommended for GitHub Codespaces)

    // Draw a red circle

    ili9225_draw_circle(lcd, 88, 110, 50, 0xF800);

    sim_mock_flush_framebuffer();  // Update browser

    sleep(1);```bash  - `ili9225_draw_text()`, `ili9225_draw_char()`

    

    // Add text# Configure and build

    ili9225_draw_text(lcd, 10, 10, "Hello!", 0x0000, 2);

    sim_mock_flush_framebuffer();  // Update browsercmake -B build```bash

}

cmake --build build

// Add to main():

int main(void) {## Installationcd simulator

    // ... initialization code ...

    # Run the test

    my_custom_test(&lcd);  // Run your test

    ./build/test_real_ili9225pip install -r requirements-web.txt

    // ... cleanup ...

}```

```

```bash```

**Important:** Always call `sim_mock_flush_framebuffer()` to update the display!

Watch your `ili9225.c` functions draw on the web display!

## Rebuilding After Changes

cd simulator

```bash

# CMake## How It Works

cmake --build build

pip install -r requirements-web.txt### Desktop Version (Local development only)

# Make

make```

```

┌─────────────────┐```

Then run `./build/test_real_ili9225` (or `./test_real_ili9225`) again.

│  test_real_     │  Your test program

## Web Interface Controls

│  ili9225.c      │  calls real ili9225.c```bash

In the browser:

- **Clear** - Clear the display└────────┬────────┘

- **Screenshot** - Save display as PNG

- **Test Buttons** - Run specific test functions         │## Quick Startcd simulator

- **Run All Tests** - Execute full test suite

         ├─────────────────┐

## Troubleshooting

         │                 │pip install -r requirements.txt

### "Connection refused" or test hangs

┌────────▼────────┐  ┌────▼──────────┐

Make sure the web simulator is running first (`./run_web.sh`).

│   ili9225.c     │  │ ili9225_mock  │### 1. Start Web Simulator```

### Port already in use

│ (real functions)│──▶│  Hardware     │

```bash

pkill -f web_simulator.py│                 │  │  Mock Layer   │

./run_web.sh

```└─────────────────┘  └────┬──────────┘



### Display not updating                          │```bash## Usage



Check that you call `sim_mock_flush_framebuffer()` after drawing operations.                          │ Captures drawing



### Build errors                          │ operations./run_web.sh



```bash                          │

# Clean and rebuild

rm -rf build                     ┌────▼──────────┐```### Standalone Mode (Demo)

cmake -B build

cmake --build build                     │ Framebuffer   │

```

                     └────┬──────────┘

## Project Structure

                          │

```

simulator/                          │ WebSocketThis starts a Flask server at `http://localhost:5000`. Open the URL in your browser.Run the simulator with a built-in demo:

├── CMakeLists.txt           # CMake build (recommended)

├── Makefile                 # Make build (legacy)                          │

├── web_simulator.py         # Flask web server

├── templates/index.html     # Web UI                     ┌────▼──────────┐

├── ili9225_mock.c/h         # Hardware mocking layer

├── test_real_ili9225.c      # Test program (EDIT THIS!)                     │ Web Simulator │

├── run_web.sh               # Launcher script

└── requirements-web.txt     # Python dependencies                     │  (Flask App)  │### 2. Run Test Program```bash

```

                     └────┬──────────┘

## Why CMake?

                          │python ili9225_simulator.py --demo

CMake matches your main project build system for consistency:

                     ┌────▼──────────┐

```

Main Project               Simulator                     │   Browser     │In another terminal:```

├── CMakeLists.txt    →   ├── CMakeLists.txt

├── src/ili9225.c     →   └── Tests real code!                     │   Display     │

```

                     └───────────────┘

## RGB565 Colors

```

Use these color constants:

```bash### Network Mode

```c

#define COLOR_BLACK   0x0000### Mock Layer

#define COLOR_WHITE   0xFFFF

#define COLOR_RED     0xF800make test

#define COLOR_GREEN   0x07E0

#define COLOR_BLUE    0x001FThe mock layer (`ili9225_mock.c`) intercepts:

#define COLOR_YELLOW  0xFFE0

#define COLOR_CYAN    0x07FF- `gpio_put()` - Tracks DC/CS pin states```Start the simulator to receive commands over TCP:

#define COLOR_MAGENTA 0xF81F

```- `spi_write_blocking()` - Captures register writes and pixel data



Or create custom colors: `RGB565(r, g, b)` where r=0-31, g=0-63, b=0-31- `sleep_ms()` - Allows visualization timing



## Advanced Usage



### Running Specific TestsIt maintains a framebuffer and sends updates to the web simulator via WebSocket.Or manually:```bash



Edit `main()` in `test_real_ili9225.c` to comment out tests:



```c## Web Interface Controlspython ili9225_simulator.py --network --port 9225

int main(void) {

    // ... init ...

    

    // Run only what you want:- **Clear** - Clear the display```bash```

    test_real_pixels(&lcd);

    // test_real_lines(&lcd);      // Commented out- **Screenshot** - Save current display as PNG

    // test_real_rectangles(&lcd);  // Commented out

    test_real_circles(&lcd);- **Test Pixels** - Run pixel drawing testmake

    

    // ... cleanup ...- **Test Lines** - Run line drawing test

}

```- **Test Rectangles** - Run rectangle test./test_real_ili9225In another terminal, run the test client:



### Custom Display Size- **Test Circles** - Run circle test



In `test_real_ili9225.c`:- **Test Text** - Run text rendering test```



```c- **Run All Tests** - Execute full test suite

ili9225_init(&lcd, spi0,

             2, 3, 4,```bash

             5, 6, 7,

             176, 220,  // Change width/height here## Project Files

             ILI9225_PORTRAIT);

```Watch your `ili9225.c` functions draw on the web display!python test_simulator.py



### Different Rotation```



```csimulator/```

// In your test:

ili9225_set_rotation(&lcd, ILI9225_LANDSCAPE);├── CMakeLists.txt            # CMake build configuration

```

├── web_simulator.py          # Flask web server with WebSocket## How It Works

Options: `ILI9225_PORTRAIT`, `ILI9225_LANDSCAPE`, `ILI9225_PORTRAIT_REV`, `ILI9225_LANDSCAPE_REV`

├── templates/

## License

│   └── index.html            # Web UI### Custom Scale

Copyright (c) 2025 trung.la

├── ili9225_mock.c/h          # Hardware mock layer

├── test_real_ili9225.c       # Test program using real ili9225.c```

├── requirements-web.txt      # Python dependencies

├── run_web.sh                # Launch script┌─────────────────┐Adjust the display zoom:

├── Makefile                  # Legacy Makefile (use CMake instead)

└── README.md                 # This file│  test_real_     │  Your test program

```

│  ili9225.c      │  calls real ili9225.c```bash

## CMake Targets

└────────┬────────┘python ili9225_simulator.py --scale 5 --demo

```bash

# Build everything         │```

cmake -B build && cmake --build build

         ├─────────────────┐

# Run test (with web simulator already running)

cmake --build build --target run_test         │                 │## Keyboard Controls



# Clean build┌────────▼────────┐  ┌────▼──────────┐

rm -rf build

```│   ili9225.c     │  │ ili9225_mock  │- **ESC** - Exit simulator



## Writing Your Own Tests│ (real functions)│──▶│  Hardware     │- **C** - Clear screen



Edit `test_real_ili9225.c` to test your drawing functions:│                 │  │  Mock Layer   │- **S** - Save screenshot (saves as `lcd_screenshot.png`)



```c└─────────────────┘  └────┬──────────┘

#include "ili9225_mock.h"

#include "../src/ili9225.h"                          │## Network Protocol

#include "../src/ili9225.c"

                          │ Captures drawing

void my_test(ili9225_config_t* lcd) {

    // Clear to white                          │ operationsThe simulator accepts JSON commands over TCP (one command per line):

    ili9225_fill_screen(lcd, 0xFFFF);

    sim_mock_flush_framebuffer();                          │

    sleep(1);

                         ┌────▼──────────┐### Clear Screen

    // Draw something

    ili9225_draw_circle(lcd, 88, 110, 50, 0xF800);                     │ Framebuffer   │```json

    sim_mock_flush_framebuffer();

}                     └────┬──────────┘{"type": "clear", "color": 0x0000}

```

                          │```

**Important**: Call `sim_mock_flush_framebuffer()` to send updates to the web display.

                          │ WebSocket

## Troubleshooting

                          │### Draw Pixel

### Port Already in Use

                     ┌────▼──────────┐```json

```bash

# Kill existing process                     │ Web Simulator │{"type": "pixel", "x": 10, "y": 20, "color": 0xF800}

pkill -f web_simulator.py

# Or use a different port                     │  (Flask App)  │```

python web_simulator.py --port 5001

```                     └────┬──────────┘



### Connection Failed                          │### Draw Line



Make sure the web simulator is running before starting the test program.                     ┌────▼──────────┐```json



### Display Not Updating                     │   Browser     │{"type": "line", "x0": 10, "y0": 20, "x1": 100, "y1": 50, "color": 0x07E0}



Check that you call `sim_mock_flush_framebuffer()` after drawing operations.                     │   Display     │```



### CMake Not Finding Files                     └───────────────┘



Make sure you're in the `simulator` directory when running CMake commands.```### Draw Rectangle



## Development```json



### Running Manually### Mock Layer{"type": "rect", "x": 10, "y": 20, "w": 50, "h": 30, "color": 0x001F}



```bash```

# Terminal 1: Start web simulator

python web_simulator.pyThe mock layer (`ili9225_mock.c`) intercepts:



# Terminal 2: Build and run test- `gpio_put()` - Tracks DC/CS pin states### Fill Rectangle

cmake -B build && cmake --build build

./build/test_real_ili9225- `spi_write_blocking()` - Captures register writes and pixel data```json

```

- `sleep_ms()` - Allows visualization timing{"type": "fill_rect", "x": 10, "y": 20, "w": 50, "h": 30, "color": 0xFFFF}

### Debug Mode

```

Enable logging in `test_real_ili9225.c`:

It maintains a framebuffer and sends updates to the web simulator via WebSocket.

```c

#define ILI9225_DEBUG_LOGGING### Draw Circle

#include "../src/ili9225.c"

```## Web Interface Controls```json



### Using Makefile (Legacy){"type": "circle", "x": 88, "y": 110, "r": 30, "color": 0xF800}



If you prefer Make over CMake:- **Clear** - Clear the display```



```bash- **Screenshot** - Save current display as PNG

make clean

make- **Test Pixels** - Run pixel drawing test### Fill Circle

./test_real_ili9225

```- **Test Lines** - Run line drawing test```json



## Why CMake?- **Test Rectangles** - Run rectangle test{"type": "fill_circle", "x": 88, "y": 110, "r": 30, "color": 0x07E0}



The simulator now uses **CMake** to maintain consistency with the main project build system. This provides:- **Test Circles** - Run circle test```

- ✅ Better integration with the main `CMakeLists.txt`

- ✅ Consistent build environment- **Test Text** - Run text rendering test

- ✅ Easier IDE integration

- ✅ Standard build practices- **Run All Tests** - Execute full test suite### Draw Text



## License```json



Copyright (c) 2025 trung.la## Project Files{"type": "text", "x": 10, "y": 100, "text": "Hello!", "color": 0xFFFF, "size": 2}


```

```

simulator/## Color Values (RGB565)

├── web_simulator.py          # Flask web server with WebSocket

├── templates/Common colors in RGB565 format:

│   └── index.html            # Web UI

├── ili9225_mock.c/h          # Hardware mock layer```python

├── test_real_ili9225.c       # Test program using real ili9225.cBLACK   = 0x0000

├── Makefile                  # Build systemWHITE   = 0xFFFF

├── requirements-web.txt      # Python dependenciesRED     = 0xF800

├── run_web.sh                # Launch scriptGREEN   = 0x07E0

└── README.md                 # This fileBLUE    = 0x001F

```YELLOW  = 0xFFE0

CYAN    = 0x07FF

## Writing Your Own TestsMAGENTA = 0xF81F

```

Edit `test_real_ili9225.c` to test your drawing functions:

## Python Client Example

```c

#include "ili9225_mock.h"```python

#include "../src/ili9225.h"from test_simulator import ILI9225Client

#include "../src/ili9225.c"

# Connect to simulator

void my_test(ili9225_config_t* lcd) {client = ILI9225Client()

    // Clear to whiteclient.connect()

    ili9225_fill_screen(lcd, 0xFFFF);

    sim_mock_flush_framebuffer();# Clear screen

    sleep(1);client.clear(client.WHITE)

    

    // Draw something# Draw shapes

    ili9225_draw_circle(lcd, 88, 110, 50, 0xF800);client.draw_pixel(10, 10, client.RED)

    sim_mock_flush_framebuffer();client.draw_line(20, 20, 100, 50, client.GREEN)

}client.draw_rect(30, 60, 50, 40, client.BLUE)

```client.fill_circle(88, 130, 25, client.MAGENTA)

client.draw_text(10, 180, "Test", client.BLACK, 2)

**Important**: Call `sim_mock_flush_framebuffer()` to send updates to the web display.

# Close connection

## Troubleshootingclient.close()

```

### Port Already in Use

## Testing

```bash

# Kill existing processRun all tests:

pkill -f web_simulator.py```bash

# Or use a different portpython test_simulator.py --test all

python web_simulator.py --port 5001```

```

Run specific test:

### Connection Failed```bash

python test_simulator.py --test shapes

Make sure the web simulator is running before starting the test program.python test_simulator.py --test animation

python test_simulator.py --test pattern

### Display Not Updating```



Check that you call `sim_mock_flush_framebuffer()` after drawing operations.## Integration with C Code



## DevelopmentTo integrate with your Pico C code:



### Running Manually1. Start the simulator in network mode

2. In your C code, establish a TCP connection to `127.0.0.1:9225`

```bash3. Send JSON commands for each drawing operation

# Terminal 1: Start web simulator4. Visualize in real-time!

python web_simulator.py

Example wrapper (pseudocode):

# Terminal 2: Build and run test```c

make cleanvoid sim_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {

make    char buf[128];

./test_real_ili9225    sprintf(buf, "{\"type\":\"pixel\",\"x\":%d,\"y\":%d,\"color\":0x%04X}\n", 

```            x, y, color);

    send(sim_socket, buf, strlen(buf), 0);

### Debug Mode}

```

Enable logging in `test_real_ili9225.c`:

## Performance

```c

#define ILI9225_DEBUG_LOGGING- Simulator runs at 60 FPS

#include "../src/ili9225.c"- Can handle thousands of drawing commands per second

```- Suitable for real-time visualization



## License## Troubleshooting



Copyright (c) 2025 trung.la### "Connection refused"

- Make sure the simulator is running in network mode
- Check the port number matches (default: 9225)

### Display looks pixelated
- Increase the scale factor: `--scale 5`

### Slow performance
- Reduce the number of individual pixel draws
- Use filled rectangles instead of many pixels
- Batch drawing operations

## Architecture

```
┌─────────────────┐
│  Your C Code    │
│  (Pico)         │
└────────┬────────┘
         │ TCP/JSON
         ▼
┌─────────────────┐
│  Simulator      │
│  (Python)       │
│  - Network      │
│  - Rendering    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Pygame Window  │
│  (Display)      │
└─────────────────┘
```

## Future Enhancements

- [ ] Serial port support
- [ ] Recording/playback of drawing sequences
- [ ] Performance profiling
- [ ] Multi-display support
- [ ] Touch input simulation
- [ ] Custom font support

## License

Part of the ILI9225 Pico library
Copyright (c) 2025 trung.la
