cmake_minimum_required(VERSION 3.13)

# Force native compilation (not cross-compile for ARM)
# This allows us to run on the development machine and connect to simulator
project(ili9225_simulator C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define SIMULATOR_BUILD for conditional compilation
add_definitions(-DSIMULATOR_BUILD)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utilities
)

# Mock library - provides hardware abstraction
add_library(ili9225_mock STATIC
    ili9225_mock.c
    ../src/utilities/log.c
)

# Test executable - tests real ili9225.c with mocked hardware
add_executable(test_real_ili9225
    test_real_ili9225.c
    ../src/ili9225.c
)

target_link_libraries(test_real_ili9225
    ili9225_mock
)

# Suppress warnings for unused parameters in unimplemented functions
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(test_real_ili9225 PRIVATE -Wno-unused-parameter)
endif()

# Install targets
install(TARGETS test_real_ili9225
    RUNTIME DESTINATION bin
)

# Custom targets for running tests with different options
add_custom_target(run_test
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --all
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running all ILI9225 simulator tests"
)

add_custom_target(test-pixels
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --pixels
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing pixel drawing functions"
)

add_custom_target(test-lines
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --lines
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing line drawing functions"
)

add_custom_target(test-rectangles
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --rectangles
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing rectangle drawing functions"
)

add_custom_target(test-circles
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --circles
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing circle drawing functions"
)

add_custom_target(test-text
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --text
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing text rendering functions"
)

add_custom_target(test-complex
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --complex
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Testing complex UI drawing"
)

add_custom_target(test-all
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225 --all
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running all tests"
)

# Print usage instructions
message(STATUS "========================================")
message(STATUS "ILI9225 Simulator - Native Build")
message(STATUS "========================================")
message(STATUS "Build:  cmake -B build && cmake --build build")
message(STATUS "Run:    ./build/test_real_ili9225 [options]")
message(STATUS "")
message(STATUS "This builds for your HOST machine (x86_64)")
message(STATUS "Tests REAL ili9225.c with mocked Pico SDK calls")
message(STATUS "Hardware calls (SPI/GPIO) are intercepted and")
message(STATUS "sent to web simulator via TCP")
message(STATUS "")
message(STATUS "Test targets:")
message(STATUS "  cmake --build build --target run_test       # Run all tests")
message(STATUS "  cmake --build build --target test-pixels    # Test pixels")
message(STATUS "  cmake --build build --target test-lines     # Test lines")
message(STATUS "  cmake --build build --target test-rectangles")
message(STATUS "  cmake --build build --target test-circles")
message(STATUS "  cmake --build build --target test-text      # Test text rendering")
message(STATUS "  cmake --build build --target test-complex   # Test complex UI")
message(STATUS "  cmake --build build --target test-all       # Run all tests")
message(STATUS "")
message(STATUS "Remember to start web simulator first:")
message(STATUS "  python3 web_simulator.py")
message(STATUS "  or: ./run_web.sh")
message(STATUS "========================================")
