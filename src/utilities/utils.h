/**
 * @file utils.h
 * @author trung.la
 * @date November 12 2025
 * @brief Utility functions and definitions
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef UTILITIES_UTILS_H
#define UTILITIES_UTILS_H

// Screen Rotation
typedef enum {
    ILI9225_PORTRAIT = 0,
    ILI9225_LANDSCAPE = 1,
    ILI9225_PORTRAIT_REV = 2,
    ILI9225_LANDSCAPE_REV = 3
} ili9225_rotation_t;

#endif // UTILITIES_UTILS_H