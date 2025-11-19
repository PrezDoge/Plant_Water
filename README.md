##  **Plant Monitoring System with Bluetooth Interface**

**UPDATED SCOPE: SENSORS AND DATA FOCUS - NO MOTOR CONTROL**

An intelligent Arduino-based plant monitoring system that uses dual soil moisture sensors and water level monitoring with Bluetooth data transmission for remote monitoring.

## Features

- **Dual Soil Sensor Monitoring**: Two moisture sensors for comprehensive soil analysis
- **Water Level Detection**: Monitor water source availability
- **Real-time Data Collection**: Continuous sensor readings with configurable intervals
- **Bluetooth Data Interface**: Wireless sensor data transmission to phone
- **Smart Device Names**: Current sensor status displayed in Bluetooth device name
- **No App Required**: Configure via native Bluetooth messaging
- **Alert Notifications**: Automatic alerts for dry soil and low water conditions
- **Data Logging**: Timestamped sensor readings and statistics
- **Remote Configuration**: Adjust thresholds and settings via Bluetooth

##  Two Operation Modes

### **Sensor Monitoring Only (NEW SCOPE)**
- **File**: `plant_monitor_sensors_only.ino`
- **Interface**: Bluetooth data transmission and alerts
- **Setup**: Pair with device, monitor sensor data
- **Features**: Real-time monitoring, alerts, data logging, NO MOTOR CONTROL

### **Legacy Full System (Original)**
- **File**: `plant_water.ino`
- **Interface**: Native phone Bluetooth popups with motor control
- **Setup**: Full system with H-bridge and motor
- **Features**: Complete watering automation (if motors are available)

## Hardware Requirements

### Core Components (NEW SCOPE - SENSORS ONLY)
- **SparkFun RedBoard (DEV-15025)** or Arduino Uno compatible
- **Two DFRobot capacitive soil moisture sensors** 
- **Water level sensor** (for monitoring water source)
- **SparkFun Pro nRF52840 Mini Bluetooth Development Board**
- **Breadboard and jumper wires**

### ~~Removed Components~~ (Motor control cut from scope)
- ~~DRV8871 H-Bridge motor driver~~
- ~~LM2596 Buck Converter~~
- ~~DC Mini Motor~~
- ~~Water pump and tubing~~

### Pin Connections (SENSORS ONLY)

| Component | RedBoard Pin | Notes |
|-----------|-------------|-------|
| Soil Moisture Sensor 1 | A0 | Analog input |
| Soil Moisture Sensor 2 | A1 | Analog input |
| Water Level Sensor | A2 | Analog input |
| SparkFun nRF52840 (TX) | Pin 4 | SoftwareSerial |
| SparkFun nRF52840 (RX) | Pin 5 | SoftwareSerial |
| Status LED | Pin 13 | Built-in LED |

### ~~Removed Connections~~ (No longer needed)
- ~~DRV8871 H-Bridge connections~~
- ~~Motor control pins~~
- ~~Manual override button~~

## Software Setup

### Arduino IDE Configuration
1. Install Arduino IDE (version 1.8.0 or newer)
2. Select board: **Arduino Uno**
3. Select correct COM port
4. Choose your version:
   - **Bluetooth Popup Interface**: Upload `plant_water_bluetooth_popup.ino` (recommended)
   - **Basic Serial**: Upload `plant_water_basic.ino`

### Dependencies
- Arduino standard libraries only (SoftwareSerial for Bluetooth)

##  Quick Start

### **Option 1: Bluetooth Popup Interface**
1. **Upload Code**: Use `plant_water.ino`
2. **Pair Device**: Look for "PlantSystem_..." in Bluetooth settings
3. **Follow Popups**: Configure via native phone notifications
4. **Get Status**: Device name shows real-time sensor readings
5. **Done**: Automatic notifications for all watering events

### **Option 2: Basic Serial**
1. **Upload Code**: Use `plant_water_basic.ino`
2. **Open Serial Monitor**: Set to 9600 baud
3. **Use Commands**: SET3V:500, SET5V:600, WATER, STOP, STATUS
4. **Monitor**: View sensor readings in serial output

## Configuration

### Bluetooth Popup Interface
- **Smart Device Names**: Status shown in Bluetooth device name
- **Menu Commands**: Send "1", "2", "3" for different options
- **Threshold Setting**: Send "THRESH3V:500" or "THRESH5V:600"
- **Manual Control**: Send "3" for watering, "4" to stop
- **Status Check**: Send "STATUS" for current readings

### Serial Commands (Basic Mode)
```
SET3V:XXX  - Set 3.3V sensor threshold
SET5V:XXX  - Set 5V sensor threshold
WATER      - Start manual watering
STOP       - Stop watering
STATUS     - Show current status
```

### Sensor Calibration
- **SEN0114 Soil Moisture**: Higher values = drier soil (inverted from some sensors)
- Dry soil: typically 600-1000
- Wet soil: typically 200-400
- Test sensor and adjust DRY_THRESHOLD accordingly

## Usage

### Without Bluetooth Module
1. **Set BLUETOOTH_ENABLED = false** in the code
2. **Automatic Mode**: System runs continuously, watering when either sensor is dry
3. **Manual Override**: Press button to force watering cycle
4. **Monitoring**: Check Serial Monitor for system status
5. **Testing Commands**: Use Serial Monitor to test Bluetooth commands

### With Bluetooth Module
1. **Set BLUETOOTH_ENABLED = true** in the code
2. **Connect nRF52840** module to pins 4 and 5
3. **Phone Control**: 
   - Set thresholds remotely: "SET_THRESHOLD_3V:500"
   - Get status: "GET_STATUS"
   - Manual watering: "MANUAL_WATER"
   - Stop watering: "STOP_WATER"
4. **Real-time Data**: Automatic updates every 10 seconds to phone
5. **Connection Status**: Monitor Bluetooth connection in system status

### Serial Monitor Output
Monitor system status through Serial Monitor at 9600 baud rate.

