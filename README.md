# Nextion Display Project

ESP32-based water level monitoring and control system with Nextion Display interface.

## Features

- Water level monitoring
- Automatic pump control based on configurable levels
- Manual and automatic operation modes
- Persistent settings storage using EEPROM
- User-friendly touch interface

## Interface Screenshots

### Main Screen
![Main Screen](Nextion%20Program/Screenshots/Dashboard_UI.png)
Shows current water level and pump status

### Settings Screen
![Settings Screen](Nextion%20Program/Screenshots/Settings_UI.png)
Configure water level thresholds and operation modes

## Hardware Requirements

- ESP32 Development Board
- Nextion Display
- Water level sensor (Ultrasonic Sensor)
- Relay (in my project DC-AC SSR)
- Power supply

## Pin Configuration

- LED Pin: GPIO2( Built_in_LED)
- Serial Communication: Default UART pins

## Settings

The following parameters can be configured through the interface:
- Off Level: Water level threshold to turn off pump
- On Level: Water level threshold to turn on pump
- Full Height: Maximum tank height reference
- Empty Height: Minimum tank height reference

## Installation

1. Clone this repository
2. Upload the Nextion HMI file to your display
3. Build and flash the ESP32 code using PlatformIO
