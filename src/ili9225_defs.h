/**
 * @file ili9225_defs.h
 * @author trung.la
 * @date November 12 2025
 * @brief Definitions for ILI9225 LCD driver for Raspberry Pi Pico
 * 
 * This header provides constants for interacting with the ILI9225 TFT LCD controller.
 * The ILI9225 is an a-Si TFT LCD Single Chip Driver with 176RGBx220 Resolution and 262K color support.
 * Based on ILI9225 Datasheet Version V0.22.
 * References: https://www.displayfuture.com/Display/datasheet/controller/ILI9225.pdf
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef ILI9225_DEFS_H
#define ILI9225_DEFS_H

// Value drive a single GPIO to high for command
#define ILI9225_CMD_GPIO 0

// Value drive a single GPIO to low for data
#define ILI9225_DATA_GPIO 1

// Its value used to set the Chip Select (CS) pin to HIGH
#define ILI9225_CS_HIGH 1 // tell the ili9225 to stop listening, release the bus for other devices

// Its value used to set the Chip Select (CS) pin to LOW
#define ILI9225_CS_LOW 0 // tell the ili9225 to start listening

#define ILI9225_LCD_WIDTH  176u
#define ILI9225_LCD_HEIGHT 220u

// Device ID code for ILI9225
#define ILI9225_DEVICE_CODE             0x9225

// Index Register (IR) - Used to specify the target register address (not a numbered register)
#define ILI9225_INDEX_REGISTER          // IR: Stores the address of the register (R00h ~ R4Fh) or RAM to access

// Status Read (RS) - Read device status and ID
#define ILI9225_STATUS_READ             // RS: Read status; Device code "9225h" is read out

// Start Oscillation (R00h)
#define ILI9225_START_OSC               0x00u
/** Start Oscillation: Controls the internal oscillator. Set OSC_EN=1 to start oscillation. Wait for stability before other settings. */

/************************************* ILI9225 LCD Registers *************************************************************/

#define ILI9225_DRIVER_OUTPUT_CTRL     0x01 // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL    0x02 // LCD AC Driving Control
#define ILI9225_ENTRY_MODE             0x03 // Entry Mode
#define ILI9225_DISP_CTRL1             0x07 // Display Control 1
#define ILI9225_BLANK_PERIOD_CTRL1     0x08 // Blank Period Control 1
#define ILI9225_FRAME_CYCLE_CTRL       0x0B // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL         0x0C // Interface Control
#define ILI9225_OSC_CTRL               0x0F // Oscillator Control
#define ILI9225_POWER_CTRL1            0x10 // Power Control 1
#define ILI9225_POWER_CTRL2            0x11 // Power Control 2
#define ILI9225_POWER_CTRL3            0x12 // Power Control 3
#define ILI9225_POWER_CTRL4            0x13 // Power Control 4
#define ILI9225_POWER_CTRL5            0x14 // Power Control 5
#define ILI9225_VCI_RECYCLING          0x15 // VCI Recycling Control
#define ILI9225_RAM_ADDR_SET1          0x20 // RAM Address Set 1
#define ILI9225_RAM_ADDR_SET2          0x21 // RAM Address Set 2
#define ILI9225_GRAM_DATA_REG          0x22 // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL         0x30 // Gate Scan Control
#define ILI9225_VERTICAL_SCROLL_CTRL1  0x31 // Vertical Scroll Control 1
#define ILI9225_VERTICAL_SCROLL_CTRL2  0x32 // Vertical Scroll Control 2
#define ILI9225_VERTICAL_SCROLL_CTRL3  0x33 // Vertical Scroll Control 3
#define ILI9225_PARTIAL_DRIVING_POS1   0x33 // Partial Driving Position 1
#define ILI9225_PARTIAL_DRIVING_POS2   0x34 // Partial Driving Position 2
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 0x35 // Horizontal Window Address 1
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 0x36 // Horizontal Window Address 2
#define ILI9225_VERTICAL_WINDOW_ADDR1  0x37 // Vertical Window Address 1
#define ILI9225_VERTICAL_WINDOW_ADDR2  0x38 // Vertical Window Address 2
#define ILI9225_GAMMA_CTRL1            0x50 // Gamma Control 1
#define ILI9225_GAMMA_CTRL2            0x51 // Gamma Control 2
#define ILI9225_GAMMA_CTRL3            0x52 // Gamma Control 3
#define ILI9225_GAMMA_CTRL4            0x53 // Gamma Control 4
#define ILI9225_GAMMA_CTRL5            0x54 // Gamma Control 5
#define ILI9225_GAMMA_CTRL6            0x55 // Gamma Control 6
#define ILI9225_GAMMA_CTRL7            0x56 // Gamma Control 7
#define ILI9225_GAMMA_CTRL8            0x57 // Gamma Control 8
#define ILI9225_GAMMA_CTRL9            0x58 // Gamma Control 9
#define ILI9225_GAMMA_CTRL10           0x59 // Gamma Control 10
/** Gamma Control: Adjusts gamma curve for grayscale. Bits like KP[5:0], RP[1:0], etc. for positive/negative polarity. */

/************************************* ILI9225 LCD Registers *************************************************************/

// OTP/MTP Related Registers
#define ILI9225_MTP_CTRL                0x81u  // MTP Control Register
/** MTP Control: For Multi-Time Programmable memory control. Bits: MTP_MODE, MTP_EX, MTP_SEL, etc. */

// Additional OTP Programming Registers
#define ILI9225_OTP_PROG_CTRL           0x61u  // Example for OTP programming
#define ILI9225_OTP_KEY                 0x63u  // Write AA55 for key
/** OTP Registers: Used for programming ID or other NV memory. */

// Entry Mode Register bits
#define ENTRY_MODE_RGB      0x0000     // RGB color order (default)
#define ENTRY_MODE_BGR      (1 << 11)  // BGR color order
#define ENTRY_MODE_MDT1     (1 << 8)   // Display shift direction
#define ENTRY_MODE_MDT0     (1 << 7)   // Display shift direction
#define ENTRY_MODE_ID1      (1 << 4)   // Increment direction bit 1
#define ENTRY_MODE_ID0      (1 << 3)   // Increment direction bit 0
#define ENTRY_MODE_AM       (1 << 2)   // Address mode (0=horizontal, 1=vertical)

#endif // ILI9225_DEFS_H