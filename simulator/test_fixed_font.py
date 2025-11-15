#!/usr/bin/env python3
"""
Simulate the FIXED font rendering to verify it's correct
"""

# Font data for test characters
font_data = {
    ' ': [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
    'H': [0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00],
    'e': [0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00],
    'l': [0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00],
    'o': [0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00],
}

def render_text(text, size=1):
    """Render text using the FIXED logic (6 columns, not 8)"""
    width = len(text) * 6 * size
    height = 8 * size
    
    # Create canvas
    canvas = [[' ' for _ in range(width)] for _ in range(height)]
    
    cur_x = 0
    for char in text:
        if char not in font_data:
            cur_x += 6 * size
            continue
        
        font = font_data[char]
        
        # FIXED: Only iterate through 6 columns (not 8!)
        for col in range(6):  # <-- This is the fix!
            line = font[col]
            for row in range(8):
                if line & 0x80:
                    # Draw pixel(s)
                    for sy in range(size):
                        for sx in range(size):
                            px = cur_x + col * size + sx
                            py = row * size + sy
                            if px < width and py < height:
                                canvas[py][px] = '█'
                line <<= 1
        
        cur_x += 6 * size  # Advance by 6 pixels
    
    return canvas

def print_canvas(canvas):
    """Print canvas"""
    for row in canvas:
        print(''.join(row))

if __name__ == "__main__":
    print("Testing FIXED font rendering (6 columns, not 8):\n")
    
    print("Size 1:")
    canvas = render_text("Hello", size=1)
    print_canvas(canvas)
    
    print("\nSize 2:")
    canvas = render_text("Hello", size=2)
    print_canvas(canvas)
