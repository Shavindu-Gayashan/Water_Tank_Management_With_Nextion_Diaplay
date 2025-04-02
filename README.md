# ESP32 Nextion Display Interface

This project demonstrates the integration of a Nextion Display with an ESP32 microcontroller, featuring LED control and variable display functionality.

## Development Environment

- Visual Studio Code
- PlatformIO Extension
- ESP32 platform

## Hardware Requirements

- ESP32 Development Board
- Nextion Display
- LED (connected to GPIO2)
- Connection cables
- USB cable for programming
- MicroSD for upload program to Nextion

## Software Dependencies

- Visual Studio Code
- PlatformIO Extension
- [Nextion Library](https://github.com/itead/ITEADLIB_Arduino_Nextion)

## Project Setup

1. **Install Prerequisites:**
   - Install Visual Studio Code
   - Install PlatformIO Extension
   - Clone this repository

2. **Open Project:**
   - Open VS Code
   - Click "Open Folder" and select the project directory
   - Wait for PlatformIO to initialize

3. **Configure platformio.ini:**
   ```ini
   [env:esp32doit-devkit-v1]
   platform = espressif32
   board = esp32doit-devkit-v1
   framework = arduino
   lib_deps = 
       itead/ITEADLIB_Arduino_Nextion@^0.9.0
   ```

## Build and Upload

1. Click the PlatformIO icon in VS Code
2. Use these commands:
   - Build: Click ✓ (Build)
   - Upload: Click → (Upload)
   - Monitor: Click 🔌 (Serial Monitor)

## Wiring Configuration

1. **Nextion Display to ESP32:**
   - Nextion TX → ESP32 GPIO16 (RX2)
   - Nextion RX → ESP32 GPIO17 (TX2)
   - VCC → 5V
   - GND → GND

2. **LED Connection:**
   - Built_in LED

## Features

- Toggle LED control with on-screen button
- Display variable values
- Real-time status updates
- Serial monitoring functionality

## Troubleshooting

1. **Serial Communication:**
   - ESP32 uses Hardware Serial2 for Nextion communication
   - Make sure TX/RX pins are correctly connected
   - Default baud rate is 9600

2. **ESP32 Specific:**
   - Use 3.3V logic level for GPIO pins
   - Built-in LED is on GPIO2
   - ESP32 has multiple serial ports available

## Project Structure

```
Nextion_Display/
├── src/
│   └── main.cpp          # Main program file
├── include/              # Header files
├── lib/                  # Project specific libraries
├── platformio.ini        # PlatformIO configuration
└── README.md            # Project documentation
```

## Contributing

Feel free to submit issues and pull requests.

## License

This project is open-source and available under the MIT License.