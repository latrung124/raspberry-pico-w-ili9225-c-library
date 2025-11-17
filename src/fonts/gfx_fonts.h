/**
 * @file gfx_fonts.h
 * @author trung.la
 * @date November 17 2025
 * @brief GFX Font definitions for ILI9225 LCD driver
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef GFX_FONTS_H
#define GFX_FONTS_H

#include "pico/stdlib.h"

// Glyph structure for proportional fonts
typedef struct {
    uint16_t bitmapOffset;  // Pointer into GFXfont->bitmap
    uint8_t  width;         // Bitmap dimensions in pixels
    uint8_t  height;        // Bitmap dimensions in pixels
    uint8_t  xAdvance;      // Distance to advance cursor (x axis)
    int8_t   xOffset;       // X dist from cursor position to UL corner
    int8_t   yOffset;       // Y dist from cursor position to UL corner
} GFXglyph;

// Font structure
typedef struct {
    const uint8_t  *bitmap;     // Glyph bitmaps, concatenated
    const GFXglyph *glyph;      // Glyph array
    uint8_t   first;            // ASCII extents (first char)
    uint8_t   last;             // ASCII extents (last char)
    uint8_t   yAdvance;         // Newline distance (y axis)
} GFXfont;

// =============================================
// LARGE FONT: PM2.5 Values (24px height)
// =============================================
static const uint8_t large_font_bitmap[] = {
    // Numbers 0-9 and decimal point, optimized for PM2.5 display
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Add your bitmap data here based on the Python font files
    // This would be converted from the Python font data
};

static const GFXglyph large_font_glyphs[] = {
    // Index, X, Y, Width, Height, XAdvance, XOffset, YOffset
    {0, 16, 24, 16, 24, 18, 1, 0},      // '0'
    {48, 8, 24, 8, 24, 18, 5, 0},       // '1'  
    {72, 16, 24, 16, 24, 18, 1, 0},     // '2'
    {120, 16, 24, 16, 24, 18, 1, 0},    // '3'
    {168, 16, 24, 16, 24, 18, 1, 0},    // '4'
    {216, 16, 24, 16, 24, 18, 1, 0},    // '5'
    {264, 16, 24, 16, 24, 18, 1, 0},    // '6'
    {312, 16, 24, 16, 24, 18, 1, 0},    // '7'
    {360, 16, 24, 16, 24, 18, 1, 0},    // '8'
    {408, 16, 24, 16, 24, 18, 1, 0},    // '9'
    {456, 4, 24, 4, 24, 8, 2, 0},       // '.' (decimal point)
};

static const GFXfont large_font = {
    large_font_bitmap,
    large_font_glyphs,
    '0',  // first
    '.',  // last
    28    // yAdvance
};

// =============================================
// MEDIUM FONT: Status Text (16px height)
// =============================================
static const uint8_t medium_font_bitmap[] = {
    // Characters: G,O,O,D,M,O,D,E,R,A,T,E,U,N,H,E,A,L,T,H,Y
    // This would contain the actual bitmap data
};

static const GFXglyph medium_font_glyphs[] = {
    {0, 12, 16, 12, 16, 14, 1, 0},   // 'G'
    {24, 12, 16, 12, 16, 14, 1, 0},  // 'O'
    {48, 8, 16, 8, 16, 10, 1, 0},    // 'D'
    {64, 14, 16, 14, 16, 16, 1, 0},  // 'M'
    {92, 12, 16, 12, 16, 14, 1, 0},  // 'E'
    {116, 10, 16, 10, 16, 12, 1, 0}, // 'R'
    {136, 10, 16, 10, 16, 12, 1, 0}, // 'A'
    {156, 12, 16, 12, 16, 14, 1, 0}, // 'T'
    {180, 12, 16, 12, 16, 14, 1, 0}, // 'U'
    {204, 8, 16, 8, 16, 10, 1, 0},   // 'N'
    {220, 10, 16, 10, 16, 12, 1, 0}, // 'H'
    {240, 8, 16, 8, 16, 10, 1, 0},   // 'L'
    {256, 10, 16, 10, 16, 12, 1, 0}, // 'Y'
};

static const GFXfont medium_font = {
    medium_font_bitmap,
    medium_font_glyphs,
    'A',  // first
    'Y',  // last
    20    // yAdvance
};

// =============================================
// SMALL FONT: Labels & Units (12px height)  
// =============================================
static const uint8_t small_font_bitmap[] = {
    // Characters for: PM2.5, TEMP, HUMIDITY, (ug/m3), °, C, %
};

static const GFXglyph small_font_glyphs[] = {
    {0, 8, 12, 8, 12, 10, 1, 0},     // 'P'
    {12, 8, 12, 8, 12, 10, 1, 0},    // 'M'
    {24, 4, 12, 4, 12, 6, 1, 0},     // '2'
    {30, 4, 12, 4, 12, 6, 1, 0},     // '.'
    {36, 4, 12, 4, 12, 6, 1, 0},     // '5'
    {42, 8, 12, 8, 12, 10, 1, 0},    // 'T'
    {54, 8, 12, 8, 12, 10, 1, 0},    // 'E'
    {66, 10, 12, 10, 12, 12, 1, 0},  // 'H'
    {81, 8, 12, 8, 12, 10, 1, 0},    // 'U'
    {93, 8, 12, 8, 12, 10, 1, 0},    // 'I'
    {105, 8, 12, 8, 12, 10, 1, 0},   // 'D'
    {117, 8, 12, 8, 12, 10, 1, 0},   // 'Y'
    {129, 4, 12, 4, 12, 6, 1, 0},    // '('
    {135, 6, 12, 6, 12, 8, 1, 0},    // 'u'
    {144, 6, 12, 6, 12, 8, 1, 0},    // 'g'
    {153, 4, 12, 4, 12, 6, 1, 0},    // '/'
    {159, 8, 12, 8, 12, 10, 1, 0},   // 'm'
    {171, 4, 12, 4, 12, 6, 1, 0},    // '3'
    {177, 4, 12, 4, 12, 6, 1, 0},    // ')'
    {183, 6, 12, 6, 12, 8, 1, 0},    // '°'
    {192, 8, 12, 8, 12, 10, 1, 0},   // 'C'
    {204, 6, 12, 6, 12, 8, 1, 0},    // '%'
};

static const GFXfont small_font = {
    small_font_bitmap,
    small_font_glyphs,
    '(',  // first
    '°',  // last  
    16    // yAdvance
};

// =============================================
// VALUE FONT: Temperature/Humidity (14px height)
// =============================================
static const uint8_t value_font_bitmap[] = {
    // Numbers 0-9, decimal point, °, C, %
};

static const GFXglyph value_font_glyphs[] = {
    {0, 10, 14, 10, 14, 12, 1, 0},   // '0'
    {18, 6, 14, 6, 14, 8, 1, 0},     // '1'
    {27, 10, 14, 10, 14, 12, 1, 0},  // '2'
    {45, 10, 14, 10, 14, 12, 1, 0},  // '3'
    {63, 10, 14, 10, 14, 12, 1, 0},  // '4'
    {81, 10, 14, 10, 14, 12, 1, 0},  // '5'
    {99, 10, 14, 10, 14, 12, 1, 0},  // '6'
    {117, 10, 14, 10, 14, 12, 1, 0}, // '7'
    {135, 10, 14, 10, 14, 12, 1, 0}, // '8'
    {153, 10, 14, 10, 14, 12, 1, 0}, // '9'
    {171, 4, 14, 4, 14, 6, 1, 0},    // '.'
    {178, 6, 14, 6, 14, 8, 1, 0},    // '°'
    {189, 8, 14, 8, 14, 10, 1, 0},   // 'C'
    {203, 6, 14, 6, 14, 8, 1, 0},    // '%'
};

static const GFXfont value_font = {
    value_font_bitmap,
    value_font_glyphs,
    '0',  // first
    '%',  // last
    18    // yAdvance
};

#endif