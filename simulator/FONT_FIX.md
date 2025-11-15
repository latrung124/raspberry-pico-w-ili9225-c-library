# Font Rendering Bug Fix

## Problem Summary
The `ili9225_draw_text()` function was rendering text incorrectly, with characters appearing garbled or overlapping.

## Root Cause
The font rendering loop was iterating through **8 columns** but then advancing the cursor by only **6 pixels**:

```c
// BUGGY CODE:
for (uint8_t col = 0; col < 8; col++) {  // ❌ Iterates 8 times
    uint8_t line = font6x8[c][col];
    for (uint8_t row = 0; row < 8; row++) {
        if (line & 0x80) {
            ili9225_draw_pixel(config, cur_x + col, cur_y + row, color);
        }
        line <<= 1;
    }
}
cur_x += 6 * size;  // ❌ But only advances 6 pixels
```

This caused a **2-pixel overlap** between characters because:
- Each character was drawn across 8 pixel columns (0-7)
- But the next character started at +6 pixels, overlapping columns 6-7 of the previous character

## Font Data Structure
The `fonts` font is defined as:
```c
static const uint8_t font6x8[96][8];
```

- 96 characters (ASCII 32-127)
- 8 bytes per character
- Each byte represents one **column** (vertical line of pixels)
- Each bit in the byte represents one **row** (bit 7 = top, bit 0 = bottom)

Most characters use columns 0-6 (7 bytes), with column 7 (byte 8) being 0x00 for spacing.
However, the font is designed to be **6 pixels wide**, not 8.

## Solution
Changed the column loop to iterate only **6 times** to match the font width and cursor advancement:

```c
// FIXED CODE:
for (uint8_t col = 0; col < 6; col++) {  // ✅ Only 6 columns
    uint8_t line = font6x8[c][col];
    for (uint8_t row = 0; row < 8; row++) {
        if (line & 0x80) {
            ili9225_draw_pixel(config, cur_x + col, cur_y + row, color);
        }
        line <<= 1;
    }
}
cur_x += 6 * size;  // ✅ Matches the 6-pixel width
```

## Files Modified
- `src/ili9225.c` - Fixed `ili9225_draw_text()` function (line ~393)

## Files Already Correct
- `src/ili9225.c` - `ili9225_draw_char()` was already using `col < 6` (line ~430)

## Testing
Run the text test to verify:
```bash
cd simulator
make test-text
```

Check the web simulator at http://localhost:5000 to see the corrected text rendering.

## Notes
- The web simulator's `draw_text()` function uses PIL/ImageFont and doesn't actually use the C font rendering logic, so it won't show this bug
- The actual bug is only visible when testing the real C code through the framebuffer
- This bug would have appeared on the physical hardware as well

## Date
November 15, 2025
