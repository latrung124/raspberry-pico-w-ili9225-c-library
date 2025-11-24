#!/usr/bin/env python3
"""
Extract and visualize text from framebuffer
"""
import sys

def rgb565_to_char(value):
    """Convert RGB565 to character for visualization"""
    if value == 0x0000:  # Black
        return '.'
    elif value == 0xFFFF:  # White
        return '█'
    elif value == 0xF800:  # Red
        return 'R'
    elif value == 0x07E0:  # Green  
        return 'G'
    elif value == 0x001F:  # Blue
        return 'B'
    elif value == 0xFFE0:  # Yellow
        return 'Y'
    elif value == 0x07FF:  # Cyan
        return 'C'
    elif value == 0xF81F:  # Magenta
        return 'M'
    else:
        return '?'

def visualize_region(width, height, x, y, w, h):
    """
    Read simulated framebuffer data and visualize a region
    For demo purposes, we'll create a test pattern
    """
    print(f"Visualizing region ({x},{y}) size {w}x{h} from {width}x{height} display")
    print()
    
    # Since we don't have direct access to the framebuffer in Python,
    # let's trace through what the C code would produce for "ILI9225 Test"
    
    # Font data for some letters
    font_data = {
        'I': [0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00],
        'L': [0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00],
        'H': [0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00],
        'e': [0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00],
        'l': [0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00],
        'o': [0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00],
    }
    
    text = "Hello"
    cur_x = 0
    
    # Create a canvas
    canvas = [[' ' for _ in range(80)] for _ in range(16)]
    
    for char in text:
        if char not in font_data:
            cur_x += 6
            continue
            
        font = font_data[char]
        
        # Render using the same logic as C code
        for col in range(8):
            line = font[col]
            for row in range(8):
                if line & 0x80:
                    if cur_x + col < 80 and row < 16:
                        canvas[row][cur_x + col] = '█'
                line <<= 1
        
        cur_x += 6  # Character spacing
    
    # Print canvas
    for row in canvas:
        print(''.join(row))

if __name__ == "__main__":
    visualize_region(176, 220, 10, 10, 120, 20)
