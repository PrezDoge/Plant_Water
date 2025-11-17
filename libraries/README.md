# Arduino Libraries

This folder is for storing custom Arduino libraries specific to this project.

## Standard Libraries Used

The main sketch uses only Arduino standard libraries:
- No additional library installations required

## Optional Libraries for Enhancement

If you want to extend the project, consider these libraries:

### LCD Display Support
- **LiquidCrystal**: For 16x2 LCD display (built-in library)
- **LiquidCrystal_I2C**: For I2C LCD displays

### Advanced Sensors
- **DHT sensor library**: For temperature/humidity monitoring
- **OneWire** & **DallasTemperature**: For DS18B20 temperature sensors

### Communication
- **WiFi**: For ESP32/ESP8266 wireless connectivity
- **Ethernet**: For wired network connectivity
- **SoftwareSerial**: For additional serial communication

### Real-Time Clock
- **RTClib**: For DS1307/DS3231 RTC modules

## Installation Instructions

### Method 1: Arduino IDE Library Manager
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries**
3. Search for library name
4. Click **Install**

### Method 2: Manual Installation
1. Download library ZIP file
2. Go to **Sketch > Include Library > Add .ZIP Library**
3. Select the downloaded ZIP file

### Method 3: Git Submodules (Advanced)
For version control of external libraries:
```bash
git submodule add https://github.com/library-repo.git libraries/LibraryName
```

## Custom Libraries

Place any custom `.h` and `.cpp` files in this directory for project-specific functions.

Example structure:
```
libraries/
├── CustomSensor/
│   ├── CustomSensor.h
│   ├── CustomSensor.cpp
│   └── examples/
└── WateringUtils/
    ├── WateringUtils.h
    └── WateringUtils.cpp
```

## Notes

- Libraries in this folder take precedence over global Arduino libraries
- Restart Arduino IDE after adding new libraries
- Check library documentation for proper usage and pin requirements