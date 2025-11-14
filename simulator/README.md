# ILI9225 Web Simulator

A web-based simulator for testing the `ili9225.c` drawing functions without physical hardware.

## OverviewTest your `ili9225.c` drawing functions in a web browser without physical hardware!

This simulator allows you to test the **real** `ili9225.c` implementation by mocking the Raspberry Pi Pico hardware layer (SPI, GPIO) and visualizing the results in a web browser in real-time.


## Architecture

```
┌─────────────────┐
│  C Code    │
│  (Pico)         │
└────────┬────────┘
         │ TCP/JSON
         ▼
┌─────────────────┐
│  Simulator      │
│  (Python)       │
│  - Network      │
│  - Rendering    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Pygame Window  │
│  (Display)      │
└─────────────────┘
```

## License

Part of the ILI9225 Pico library
Copyright (c) 2025 trung.la
