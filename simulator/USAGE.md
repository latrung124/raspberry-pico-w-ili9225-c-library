# Testing ILI9225 Functions with Command-Line Options

The `test_real_ili9225` program now supports command-line options to run specific tests or all tests.

## Quick Start

1. **Start the web simulator:**
   ```bash
   cd simulator
   python web_simulator.py
   # Or use: ./run_web.sh
   ```

2. **Build the test program:**
   ```bash
   make
   ```

3. **Run tests:**
   ```bash
   # Run all tests (default)
   ./test_real_ili9225
   
   # Or use make targets
   make test
   ```

## Command-Line Options

### Test Selection Options

- `--all` - Run all tests (this is the default if no options specified)
- `--pixels` - Test pixel drawing (`ili9225_draw_pixel`)
- `--lines` - Test line drawing (`ili9225_draw_line`)
- `--rectangles` - Test rectangle drawing (`ili9225_draw_rect`, `ili9225_fill_rect`)
- `--circles` - Test circle drawing (`ili9225_draw_circle`, `ili9225_fill_circle`)
- `--text` - Test text rendering (`ili9225_draw_text`, `ili9225_draw_char`)
- `--complex` - Test complex UI drawing (combination of shapes and text)
- `--fill-screen COLOR` - Fill the entire screen with a specified color
- `--help` - Display help message

### Color Options for `--fill-screen`

You can specify colors by name or hex value:

**Color Names:**
- `black` - 0x0000
- `white` - 0xFFFF
- `red` - 0xF800
- `green` - 0x07E0
- `blue` - 0x001F
- `yellow` - 0xFFE0
- `cyan` - 0x07FF
- `magenta` - 0xF81F

**Hex Values:**
You can also specify any RGB565 color using hex notation (e.g., `0xF800`)

## Usage Examples

### Basic Usage

```bash
# Run all tests
./test_real_ili9225
./test_real_ili9225 --all
make test

# Show help
./test_real_ili9225 --help
```

### Run Specific Tests

```bash
# Test only pixel drawing
./test_real_ili9225 --pixels
make test-pixels

# Test only line drawing
./test_real_ili9225 --lines
make test-lines

# Test only rectangle drawing
./test_real_ili9225 --rectangles
make test-rectangles

# Test only circle drawing
./test_real_ili9225 --circles
make test-circles

# Test only text rendering
./test_real_ili9225 --text
make test-text

# Test only complex UI
./test_real_ili9225 --complex
make test-complex
```

### Combine Multiple Tests

```bash
# Test pixels and lines only
./test_real_ili9225 --pixels --lines

# Test shapes (rectangles and circles)
./test_real_ili9225 --rectangles --circles

# Test drawing primitives
./test_real_ili9225 --pixels --lines --rectangles --circles
```

### Fill Screen Tests

```bash
# Fill screen with named colors
./test_real_ili9225 --fill-screen black
./test_real_ili9225 --fill-screen red
./test_real_ili9225 --fill-screen blue
./test_real_ili9225 --fill-screen white

# Fill screen with custom hex color
./test_real_ili9225 --fill-screen 0xF800  # Red
./test_real_ili9225 --fill-screen 0x07E0  # Green
./test_real_ili9225 --fill-screen 0x001F  # Blue
./test_real_ili9225 --fill-screen 0xFFE0  # Yellow
```

### Advanced Combinations

```bash
# Test circles then fill screen with red
./test_real_ili9225 --circles --fill-screen red

# Test all drawing primitives except text
./test_real_ili9225 --pixels --lines --rectangles --circles

# Test text and complex UI
./test_real_ili9225 --text --complex
```

## Makefile Targets

For convenience, you can use these make targets:

```bash
make              # Build the test program
make clean        # Remove built files
make help         # Show help information

# Test targets
make test         # Run all tests
make test-all     # Run all tests
make test-pixels  # Test pixel drawing
make test-lines   # Test line drawing
make test-rectangles   # Test rectangle drawing
make test-circles      # Test circle drawing
make test-text         # Test text rendering
make test-complex      # Test complex UI
```

## What Each Test Does

### `--pixels` Test
- Fills screen with white
- Draws a red diagonal line pixel by pixel (100 pixels)
- Tests `ili9225_draw_pixel()`

### `--lines` Test
- Fills screen with black
- Draws horizontal line in red
- Draws diagonal line in green
- Draws another diagonal line in blue
- Tests `ili9225_draw_line()`

### `--rectangles` Test
- Fills screen with black
- Draws outline rectangle in red
- Draws filled rectangle in green
- Draws outline rectangle in blue
- Draws filled rectangle in yellow
- Tests `ili9225_draw_rect()` and `ili9225_fill_rect()`

### `--circles` Test
- Fills screen with white, draws concentric outline circles
- Fills screen with black, draws filled circles
- Tests `ili9225_draw_circle()` and `ili9225_fill_circle()`

### `--text` Test
- Displays various text strings at different sizes
- Draws individual characters
- Tests `ili9225_draw_text()` and `ili9225_draw_char()`

### `--complex` Test
- Creates a complete UI mockup with:
  - Title bar (blue background)
  - Content area with shapes
  - Status bar (black background)
- Tests combination of all drawing functions

### `--fill-screen COLOR` Test
- Fills the entire screen with the specified color
- Useful for testing display initialization and basic rendering
- Tests `ili9225_fill_screen()`

## Viewing Results

1. Open the web simulator in your browser at `http://localhost:5000`
2. The display updates in real-time as tests run
3. Each test includes a 1-2 second delay between steps so you can see the progression

## Tips

- **Run the simulator first** before running tests, otherwise tests will skip flushing
- **Use specific tests** during development to iterate quickly
- **Combine tests** to verify multiple functions work together
- **Check the web browser** to see visual results
- The test program will indicate connection status and test progress in the terminal

## Troubleshooting

### "Connection refused" error
- Make sure the web simulator is running (`python web_simulator.py`)
- Check that port 9225 is available

### Display not updating
- Ensure your browser is open to `http://localhost:5000`
- Check browser console for errors
- Verify the simulator shows "C client connected" message

### Tests run but display is blank
- Check that `sim_mock_flush_framebuffer()` is being called
- Verify the simulator received the data (check terminal output)
- Try refreshing the web browser
