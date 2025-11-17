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
    ILI9225_PORTRAIT = 0,                   // AM=0, ID=0b10 - Normal portrait
    ILI9225_LANDSCAPE = 1,                  // AM=1, ID=0b10 - Normal landscape 
    ILI9225_PORTRAIT_REV = 2,               // AM=0, ID=0b00 - Portrait reversed
    ILI9225_LANDSCAPE_REV = 3,              // AM=1, ID=0b00 - Landscape reversed
    ILI9225_PORTRAIT_MIRROR = 4,            // AM=0, ID=0b11 - Portrait mirrored
    ILI9225_LANDSCAPE_MIRROR = 5,           // AM=1, ID=0b11 - Landscape mirrored
    ILI9225_PORTRAIT_REV_MIRROR = 6,        // AM=0, ID=0b01 - Portrait mirrored & reversed
    ILI9225_LANDSCAPE_REV_MIRROR = 7        // AM=1, ID=0b01 - Landscape mirrored & reversed
} ili9225_rotation_t;

// Colors
#define COLOR_BLACK          0x0000
#define COLOR_WHITE          0xFFFF
#define COLOR_BLUE           0x001F
#define COLOR_GREEN          0x07E0
#define COLOR_RED            0xF800
#define COLOR_NAVY           0x000F
#define COLOR_DARKBLUE       0x0011
#define COLOR_DARKGREEN      0x03E0
#define COLOR_DARKCYAN       0x03EF
#define COLOR_CYAN           0x07FF
#define COLOR_TURQUOISE      0x471A
#define COLOR_INDIGO         0x4810
#define COLOR_DARKRED        0x8000
#define COLOR_OLIVE          0x7BE0
#define COLOR_GRAY           0x8410
#define COLOR_GREY           0x8410
#define COLOR_SKYBLUE        0x867D
#define COLOR_BLUEVIOLET     0x895C
#define COLOR_LIGHTGREEN     0x9772
#define COLOR_DARKVIOLET     0x901A
#define COLOR_YELLOWGREEN    0x9E66
#define COLOR_BROWN          0xA145
#define COLOR_DARKGRAY       0x7BEF
#define COLOR_DARKGREY       0x7BEF
#define COLOR_SIENNA         0xA285
#define COLOR_LIGHTBLUE      0xAEDC
#define COLOR_GREENYELLOW    0xAFE5
#define COLOR_SILVER         0xC618
#define COLOR_LIGHTGRAY      0xC618
#define COLOR_LIGHTCYAN      0xE7FF
#define COLOR_VIOLET         0xEC1D
#define COLOR_AZUR           0xF7FF
#define COLOR_BEIGE          0xF7BB
#define COLOR_MAGENTA        0xF81F
#define COLOR_TOMATO         0xFB08
#define COLOR_GOLD           0xFEA0
#define COLOR_ORANGE         0xFD20
#define COLOR_SNOW           0xFFDF
#define COLOR_YELLOW         0xFFE0

#endif // UTILITIES_UTILS_H