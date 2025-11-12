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

// Driver Output Control (R01h)
#define ILI9225_DRIVER_OUTPUT_CTRL      0x01u
/** Driver Output Control: Sets scanning direction, source/gate driver settings, etc. Bits include SM, GS, SS, etc. */

// LCD Driving Waveform Control (R02h)
#define ILI9225_LCD_DRIVING_WAVE        0x02u
/** LCD Driving Waveform Control: Controls the LCD AC driving waveform, frame inversion, etc. Bits: B/C, EOR, NW, etc. */

// Entry Mode (R03h)
#define ILI9225_ENTRY_MODE              0x03u
/** Entry Mode: Sets GRAM access direction, data format, etc. Bits: AM, ID[1:0], ORG, etc. */

// Resize Control (R04h) - If present in some variants
#define ILI9225_RESIZE_CTRL             0x04u
/** Resize Control: Controls resizing functions (if supported). */

// Display Control 1 (R07h)
#define ILI9225_DISP_CTRL1              0x07u
/** Display Control 1: Main display control, including BASEE, GON, DTE, D[1:0] for on/off. */

// Display Control 2 (R08h)
#define ILI9225_DISP_CTRL2              0x08u
/** Display Control 2: Controls blank period, etc. */

// Display Control 3 (R09h) - If applicable
#define ILI9225_DISP_CTRL3              0x09u
/** Display Control 3: Additional display settings. */

// Display Control 4 (R0Ah) - If applicable
#define ILI9225_DISP_CTRL4              0x0A
/** Display Control 4: Further display controls. */

// Frame Cycle Control (R0Bh)
#define ILI9225_FRAME_CYCLE_CTRL        0x0B
/** Frame Cycle Control: Sets non-display area driving periods. */

// RGB Display Interface Control 1 (R0Ch)
#define ILI9225_RGB_DISP_IF_CTRL1       0x0C
/** RGB Display Interface Control 1: Controls RGB interface settings. Bits: RM, RIM[2:0], etc. */

// Frame Marker Position (R0Dh)
#define ILI9225_FRAME_MARKER_POS        0x0D
/** Frame Marker Position: Sets frame marker position. */

// RGB Display Interface Control 2 (R0Fh) - Note: 0x0F might be Oscillator in some
#define ILI9225_RGB_DISP_IF_CTRL2       0x0F
/** RGB Display Interface Control 2: Additional RGB interface controls. */

// Oscillator Control (R0Fh) - Conflicts in some sources; verify
#define ILI9225_OSC_CTRL                0x0F
/** Oscillator Control: Controls internal oscillator frequency. */

// Power Control 1 (R10h)
#define ILI9225_POWER_CTRL1             0x10u
/** Power Control 1: Sets power modes, SAP, BT[2:0], APE, etc. */

// Power Control 2 (R11h)
#define ILI9225_POWER_CTRL2             0x11u
/** Power Control 2: DC[3:0], VC[2:0], etc. for step-up circuits. */

// Power Control 3 (R12h)
#define ILI9225_POWER_CTRL3             0x12u
/** Power Control 3: VRH[3:0] for VREG1OUT voltage. */

// Power Control 4 (R13h)
#define ILI9225_POWER_CTRL4             0x13u
/** Power Control 4: VDV[4:0] for VCOM amplitude. */

// Power Control 5 (R14h)
#define ILI9225_POWER_CTRL5             0x14u
/** Power Control 5: VCM[6:0] for VCOMH, VML[6:0] for VCOML. */

// VCI Recycling (R15h)
#define ILI9225_VCI_RECYCLING           0x15u
/** VCI Recycling: Controls VCI recycling function. */

// RAM Address Set (Horizontal/Vertical)
#define ILI9225_RAM_ADDR_SET1           0x20u  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           0x21u  // Vertical GRAM Address Set
/** RAM Address Set: Sets the GRAM address for access (AD[15:0]). */

// GRAM Data Register (R22h)
#define ILI9225_GRAM_DATA_REG           0x22u
/** GRAM Data Register: Access port for writing/reading GRAM data. Auto-increments address counter. */

// Software Reset (R28h)
#define ILI9225_SOFTWARE_RESET          0x28u
/** Software Reset: Resets the device to initial state. */

// Gate Scan Control (R30h)
#define ILI9225_GATE_SCAN_CTRL          0x30u
/** Gate Scan Control: Controls gate scanning. */

// Vertical Scroll Control (R31h-R33h)
#define ILI9225_VERTICAL_SCROLL_CTRL1   0x31u
#define ILI9225_VERTICAL_SCROLL_CTRL2   0x32u
#define ILI9225_VERTICAL_SCROLL_CTRL3   0x33u
/** Vertical Scroll Control: Sets scroll parameters. */

// Partial Driving Position (R34h-R35h)
#define ILI9225_PARTIAL_DRIVING_POS1    0x34u
#define ILI9225_PARTIAL_DRIVING_POS2    0x35u
/** Partial Driving Position: Sets partial display areas. */

// Horizontal Window Address (R36h-R37h)
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 0x36u  // HSA[7:0]
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 0x37u  // HEA[7:0]
/** Horizontal Window Address: Sets horizontal start/end for window. */

// Vertical Window Address (R38h-R39h)
#define ILI9225_VERTICAL_WINDOW_ADDR1   0x38u  // VSA[8:0]
#define ILI9225_VERTICAL_WINDOW_ADDR2   0x39u  // VEA[8:0]
/** Vertical Window Address: Sets vertical start/end for window. */

// Gamma Control Registers (R50h-R59h)
#define ILI9225_GAMMA_CTRL1             0x50u
#define ILI9225_GAMMA_CTRL2             0x51u
#define ILI9225_GAMMA_CTRL3             0x52u
#define ILI9225_GAMMA_CTRL4             0x53u
#define ILI9225_GAMMA_CTRL5             0x54u
#define ILI9225_GAMMA_CTRL6             0x55u
#define ILI9225_GAMMA_CTRL7             0x56u
#define ILI9225_GAMMA_CTRL8             0x57u
#define ILI9225_GAMMA_CTRL9             0x58u
#define ILI9225_GAMMA_CTRL10            0x59u
/** Gamma Control: Adjusts gamma curve for grayscale. Bits like KP[5:0], RP[1:0], etc. for positive/negative polarity. */

/************************************* ILI9225 LCD Registers *************************************************************/

// OTP/MTP Related Registers
#define ILI9225_MTP_CTRL                0x81u  // MTP Control Register
/** MTP Control: For Multi-Time Programmable memory control. Bits: MTP_MODE, MTP_EX, MTP_SEL, etc. */

// Additional OTP Programming Registers
#define ILI9225_OTP_PROG_CTRL           0x61u  // Example for OTP programming
#define ILI9225_OTP_KEY                 0x63u  // Write AA55 for key
/** OTP Registers: Used for programming ID or other NV memory. */

// Bit Masks Examples (for common registers)
// For Entry Mode (R03h)
#define ILI9225_ENTRY_MODE_AM_VERTICAL  (1 << 3)  // AM=1 for vertical addressing


#endif // ILI9225_DEFS_H