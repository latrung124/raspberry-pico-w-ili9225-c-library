#!/usr/bin/env python3
"""
Analyze font data to see which columns actually have data
"""

# Sample of font data from terminal6x8.h
font_samples = {
    'space': [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
    '!':     [0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18],
    'A':     [0x3C, 0x66, 0x6E, 0x7E, 0x66, 0x66, 0x66, 0x00],
    'H':     [0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00],
    'W':     [0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00],
    'a':     [0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00],
    '0':     [0x3E, 0x63, 0x6B, 0x6B, 0x63, 0x3E, 0x00, 0x00],
}

def analyze_font_width():
    print("Analyzing font data structure:\n")
    
    for char, data in font_samples.items():
        print(f"Character '{char}':")
        print(f"  Data: [{', '.join(f'0x{b:02X}' for b in data)}]")
        
        # Check which columns have any data
        non_zero_cols = [i for i, b in enumerate(data) if b != 0x00]
        print(f"  Non-zero columns: {non_zero_cols}")
        print(f"  Effective width: {max(non_zero_cols) + 1 if non_zero_cols else 0} pixels")
        
        # Visualize
        print(f"  Visual (8 rows x 8 columns):")
        for row in range(8):
            print(f"    Row {row}: ", end='')
            for col in range(8):
                byte_val = data[col]
                if byte_val & (0x80 >> row):
                    print('█', end='')
                else:
                    print('·', end='')
            print()
        print()

if __name__ == "__main__":
    analyze_font_width()
