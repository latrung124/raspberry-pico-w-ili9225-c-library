# Display Orientation Fix - Upside Down Text

## Problem
Text and graphics displayed in the web simulator appeared **upside down** (rotated 180 degrees).

## Root Cause

The ILI9225 display controller has different coordinate systems depending on the **Entry Mode** setting.

### Entry Mode in Portrait (0x0018)
- Binary: `00011000`
- Bit 3 (AM): 1 = Vertical address increment
- Bit 4 (ID1): 1 = Increment direction
- **Origin position**: Bottom-right corner
- **Address increment**: Vertically upward, then left

### Framebuffer Format
The C code stores pixels in **row-major order**:
```
framebuffer[y * width + x]  // y=0 is at index 0 (top)
```

### Web Simulator Assumption (WRONG)
The original code assumed standard top-left origin:
```python
pixels[x, y] = framebuffer_data  # Maps y=0 to top of screen
```

This caused:
- Framebuffer y=0 (intended top) → Display y=0 (actual bottom in ILI9225)
- **Result**: Everything upside down!

## Solution

Flip the framebuffer vertically when rendering:

```python
# BEFORE (wrong):
pixels[x, y] = framebuffer_data

# AFTER (correct):
pixels[x, height - 1 - y] = framebuffer_data
```

This maps:
- Framebuffer y=0 → Display y=219 (bottom)
- Framebuffer y=219 → Display y=0 (top)

## Files Modified
- `simulator/web_simulator.py` - Added vertical flip in `update_from_framebuffer()`

## Visual Explanation

```
C Code Framebuffer (row-major):          Web Display (with fix):
┌─────────────────────┐                 ┌─────────────────────┐
│ y=0:   Hello World  │ ───────────────>│ y=0:   Hello World  │
│ y=1:   Text here    │    flip y       │ y=1:   Text here    │
│ y=2:   ...          │                 │ y=2:   ...          │
└─────────────────────┘                 └─────────────────────┘

Without fix: Text appeared as "plɹoM ollǝH" (upside down)
With fix: Text appears as "Hello World" (correct)
```

## Technical Details

The ILI9225 Entry Mode register (R03h) controls:
- **AM (Address Mode)**: Horizontal vs Vertical increment
- **ID1/ID0 (Increment Direction)**: Which direction to move after each pixel
- **ORG (Origin)**: Implicit based on AM and ID bits

For portrait mode (0x0018):
- Origin is conceptually at bottom-right
- Address increments upward, then left
- Our framebuffer assumes top-left origin
- **Fix**: Flip Y coordinate to match hardware behavior

## Testing

Run any test to verify:
```bash
./test_real_ili9225 --text
```

Open http://localhost:5000 - text should appear **right-side up**.

## Notes

- This fix is specific to the web simulator's visual representation
- The actual ILI9225 hardware handles this internally via entry mode
- Other rotation modes (landscape, portrait_rev, landscape_rev) may need different transforms
- Current fix assumes portrait mode (0x0018) which is the default

## Date
November 16, 2025
