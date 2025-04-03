# Nextion Display Control Project

This project implements a control system using an ESP32 microcontroller and Nextion display interface. It features both automatic and manual control modes with configurable parameters stored in EEPROM.

## Features

- Dual control modes (Automatic/Manual)
- Configurable level parameters
  - On Level
  - Off Level
  - Full Height
  - Empty Height
- Persistent storage using EEPROM
- Real-time parameter updates
- User-friendly touch interface

## Hardware Requirements

- ESP32 Development Board
- Nextion Display
- LED indicator (connected to pin 2)

## Software Dependencies

- Arduino IDE
- Nextion Library
- EEPROM Library

## Pin Configuration

- LED Pin: GPIO 2
- Nextion Serial: Default UART pins (TX: GPIO1, RX: GPIO3)

## Getting Started

1. Connect the hardware according to pin configuration
2. Install required libraries in Arduino IDE
3. Upload the code to ESP32
4. Power up the system

## Settings

The system allows configuration of:
- Water level parameters
- Auto/Manual mode settings
- On/Off state configurations

All settings are preserved in EEPROM memory.

## License

This project is open source and available under the MIT License.
