cmake_minimum_required(VERSION 3.13)

project(ili9225_simulator C)

set(CMAKE_C_STANDARD 11)
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

# Add custom target to run the test
add_custom_target(run_test
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_real_ili9225
    DEPENDS test_real_ili9225
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running ILI9225 simulator test"
)

# Print usage instructions
message(STATUS "========================================")
message(STATUS "ILI9225 Web Simulator Build")
message(STATUS "========================================")
message(STATUS "Build: cmake -B build && cmake --build build")
message(STATUS "Run:   ./build/test_real_ili9225")
message(STATUS "")
message(STATUS "Remember to start web simulator first:")
message(STATUS "  ./run_web.sh")
message(STATUS "========================================")
