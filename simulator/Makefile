# ============================================================================
# Legacy Makefile for ILI9225 Web Simulator
# 
# NOTE: This project now uses CMake as the primary build system.
# Use CMake for better integration with the main project:
#   cmake -B build && cmake --build build
#
# This Makefile is kept for convenience but may not be maintained.
# ============================================================================

# Makefile for ILI9225 Web Simulator
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I. -I../src -I../src/fonts -I../src/utilities -DSIMULATOR_BUILD
LDFLAGS = 

# Source files
MOCK_SRC = ili9225_mock.c
LOG_SRC = ../src/utilities/log.c
TEST_REAL_SRC = test_real_ili9225.c

# Object files
MOCK_OBJ = $(MOCK_SRC:.c=.o)
LOG_OBJ = log.o

# Target binary
TEST_REAL_BIN = test_real_ili9225

.PHONY: all clean test help

all: $(TEST_REAL_BIN)

$(TEST_REAL_BIN): $(TEST_REAL_SRC) $(MOCK_OBJ) $(LOG_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LOG_OBJ): $(LOG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_REAL_BIN) $(MOCK_OBJ) $(LOG_OBJ)

test: $(TEST_REAL_BIN)
	@echo "Starting web simulator and test program..."
	@echo "Make sure to run './run_web.sh' in another terminal first!"
	./$(TEST_REAL_BIN)

help:
	@echo "ILI9225 Web Simulator Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all         - Build test program (default)"
	@echo "  clean       - Remove built files"
	@echo "  test        - Build and run test (requires web simulator running)"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  1. Start web simulator: ./run_web.sh"
	@echo "  2. In another terminal: make test"
	@echo ""
	@echo "Usage:"
	@echo "  1. Start simulator: python3 web_simulator.py --tcp-port 9225"
	@echo "  2. Build tests: make"
	@echo "  3. Run REAL ili9225.c test: make run-real"
	@echo ""
	@echo "The 'test_real_ili9225' program tests your actual ili9225.c code!"
