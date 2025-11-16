# CMake Build System for ILI9225 Simulator

## Overview
The simulator can be built using either **Make** or **CMake**. Both build systems produce the same test executable that connects to the web simulator.

## Quick Start with CMake

### 1. Configure and Build
```bash
cmake -B build
cmake --build build
```

### 2. Run Tests
```bash
# Run the executable directly
./build/test_real_ili9225 --help

# Or use CMake targets
cmake --build build --target test-all
```

## CMake Test Targets

The CMakeLists.txt provides convenient targets for running different tests:

| Target | Command | Description |
|--------|---------|-------------|
| `run_test` | `cmake --build build --target run_test` | Run all tests |
| `test-all` | `cmake --build build --target test-all` | Run all tests |
| `test-pixels` | `cmake --build build --target test-pixels` | Test pixel drawing |
| `test-lines` | `cmake --build build --target test-lines` | Test line drawing |
| `test-rectangles` | `cmake --build build --target test-rectangles` | Test rectangle drawing |
| `test-circles` | `cmake --build build --target test-circles` | Test circle drawing |
| `test-text` | `cmake --build build --target test-text` | Test text rendering |
| `test-complex` | `cmake --build build --target test-complex` | Test complex UI |

## Examples

```bash
# Build and run all tests
cmake -B build && cmake --build build --target run_test

# Test only text rendering
cmake --build build --target test-text

# Test multiple features
./build/test_real_ili9225 --pixels --lines --text

# Fill screen with color
./build/test_real_ili9225 --fill-screen blue

# Get help
./build/test_real_ili9225 --help
```

## Clean and Rebuild

```bash
# Clean build directory
rm -rf build

# Reconfigure and build
cmake -B build
cmake --build build
```

## CMake vs Make

Both build systems are maintained and equivalent:

| Feature | CMake | Make |
|---------|-------|------|
| Build command | `cmake --build build` | `make` |
| Clean | `rm -rf build` | `make clean` |
| Test targets | `cmake --build build --target test-text` | `make test-text` |
| IDE support | ✅ Generates IDE project files | ❌ No IDE support |
| Dependencies | Automatic | Manual |

**Choose CMake if:**
- You're using an IDE (VS Code, CLion, etc.)
- You want better dependency tracking
- You need cross-platform builds

**Choose Make if:**
- You prefer simple, direct commands
- You're familiar with Makefiles
- You want faster incremental builds

## Web Simulator

Don't forget to start the web simulator before running tests:

```bash
# In a separate terminal
python3 web_simulator.py

# Or use the wrapper script
./run_web.sh
```

Then open http://localhost:5000 in your browser to see the LCD display.

## Troubleshooting

### "Connection refused" error
The web simulator is not running. Start it with:
```bash
python3 web_simulator.py
```

### Build errors about multiple definitions
Run a clean build:
```bash
rm -rf build
cmake -B build && cmake --build build
```

### Tests run but display is not updated
1. Check that web simulator is running
2. Refresh the browser at http://localhost:5000
3. Check for errors in simulator output
