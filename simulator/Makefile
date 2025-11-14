# Makefile for ILI9225 Web Simulator Test Program

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I. -I../src -I../src/fonts -I../src/utilities -DSIMULATOR_BUILD
LDFLAGS = 

# Source files
MOCK_SRC = ili9225_mock.c
DRIVER_SRC = ../src/ili9225.c
LOG_SRC = ../src/utilities/log.c
TEST_SRC = test_real_ili9225.c

# Target binary
TEST_BIN = test_real_ili9225

.PHONY: all clean test help test-all test-pixels test-lines test-rectangles test-circles test-text test-complex

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(MOCK_SRC) $(DRIVER_SRC) $(LOG_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TEST_BIN) *.o

# Test targets with command-line options
test: $(TEST_BIN)
	@echo "Starting web simulator test with all tests..."
	@echo "Make sure the web simulator is running (./run_web.sh or python web_simulator.py)"
	./$(TEST_BIN) --all

test-pixels: $(TEST_BIN)
	@echo "Testing pixel drawing functions..."
	./$(TEST_BIN) --pixels

test-lines: $(TEST_BIN)
	@echo "Testing line drawing functions..."
	./$(TEST_BIN) --lines

test-rectangles: $(TEST_BIN)
	@echo "Testing rectangle drawing functions..."
	./$(TEST_BIN) --rectangles

test-circles: $(TEST_BIN)
	@echo "Testing circle drawing functions..."
	./$(TEST_BIN) --circles

test-text: $(TEST_BIN)
	@echo "Testing text rendering functions..."
	./$(TEST_BIN) --text

test-complex: $(TEST_BIN)
	@echo "Testing complex UI drawing..."
	./$(TEST_BIN) --complex

test-all: $(TEST_BIN)
	@echo "Running all tests..."
	./$(TEST_BIN) --all

help:
	@echo "ILI9225 Web Simulator Test Program"
	@echo ""
	@echo "Build targets:"
	@echo "  all              - Build test program (default)"
	@echo "  clean            - Remove built files"
	@echo ""
	@echo "Test targets:"
	@echo "  test             - Run all tests"
	@echo "  test-all         - Run all tests"
	@echo "  test-pixels      - Test pixel drawing"
	@echo "  test-lines       - Test line drawing"
	@echo "  test-rectangles  - Test rectangle drawing"
	@echo "  test-circles     - Test circle drawing"
	@echo "  test-text        - Test text rendering"
	@echo "  test-complex     - Test complex UI drawing"
	@echo ""
	@echo "Usage:"
	@echo "  1. Start web simulator: ./run_web.sh or python web_simulator.py"
	@echo "  2. Build: make"
	@echo "  3. Run tests: make test (or make test-pixels, etc.)"
	@echo ""
	@echo "Advanced usage:"
	@echo "  ./test_real_ili9225 --help    # See all command-line options"
	@echo "  ./test_real_ili9225 --fill-screen red"
	@echo "  ./test_real_ili9225 --pixels --lines --circles"
