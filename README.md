## 🌱 **Automated Plant Watering System with Bluetooth Interface**

An intelligent Arduino-based plant watering system that uses dual moisture sensors and native Bluetooth popup interface for monitoring and control.
## Features

- **Dual Sensor Monitoring**: Two moisture sensors (3.3V and 5V) for redundancy
- **H-Bridge Motor Control**: Professional motor control with DRV8871
- **Bluetooth Popup Interface**: Native phone notifications and configuration popups
- **Smart Device Names**: Status displayed in Bluetooth device name
- **No App Required**: Configure via native Bluetooth messaging
- **Real-time Notifications**: Automatic alerts for watering events
- **Manual Override**: Physical button and Bluetooth controls
- **Simple Menu System**: Number-based configuration interface

## 🔵 Two Operation Modes

### **Bluetooth Popup Interface** (Recommended)
- **File**: `plant_water.ino`
- **Interface**: Native phone Bluetooth popups and notifications
- **Setup**: Pair with device, follow popup menus
- **Features**: Smart device names, automatic notifications, simple configuration

### **Basic Serial Mode**
- **File**: `plant_water_basic.ino`
- **Interface**: Arduino IDE Serial Monitor
- **Setup**: Upload code and use serial commands
- **Features**: Serial configuration, manual controls

## Hardware Requirements

### Components
- SparkFun RedBoard (DEV-13975) or Arduino Uno compatible
- Two moisture sensors (3.3V and 5V versions)
- DRV8871 H-Bridge motor driver (6.5V-45V, 3.6A capacity)
- LM2596 Buck Converter (3V-40V input, 1.25V-35V output)
- DC Mini Motor (1.5V-6V, 0.4A-0.55A capacity)
- SparkFun Pro nRF52840 Mini Bluetooth Development Board
- Breadboard and jumper wires  
- Water reservoir and tubing

### SparkFun Pro nRF52840 Mini Benefits
- **USB-C Programming**: Direct programming via Arduino IDE
- **Built-in Voltage Regulation**: Clean 3.3V power management
- **Professional PCB**: Reliable connections and debugging
- **SparkFun Documentation**: Excellent tutorials and support
- **Arduino IDE Compatible**: Easy development workflow

### Pin Connections

| Component | RedBoard Pin | Notes |
|-----------|-------------|-------|
| 3.3V Moisture Sensor | A0 | Analog input |
| 5V Moisture Sensor | A1 | Analog input |
| DRV8871 H-Bridge (IN1) | Pin 7 | Motor control |
| DRV8871 H-Bridge (IN2) | Pin 8 | Motor control |
| SparkFun nRF52840 (TX) | Pin 4 | SoftwareSerial |
| SparkFun nRF52840 (RX) | Pin 5 | SoftwareSerial |
| Manual Override Button | Pin 2 | Digital input with pull-up |
| Status LED | Pin 13 | Built-in LED |

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

## 🚀 Quick Start

### **Option 1: Bluetooth Popup Interface (Recommended)**
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

### Without Bluetooth Module (Current Setup)
1. **Set BLUETOOTH_ENABLED = false** in the code
2. **Automatic Mode**: System runs continuously, watering when either sensor is dry
3. **Manual Override**: Press button to force watering cycle
4. **Monitoring**: Check Serial Monitor for system status
5. **Testing Commands**: Use Serial Monitor to test Bluetooth commands

### With Bluetooth Module (When Available)
1. **Set BLUETOOTH_ENABLED = true** in the code
2. **Connect nRF52840** module to pins 4 and 5
3. **Phone App Control**: 
   - Set thresholds remotely: "SET_THRESHOLD_3V:500"
   - Get status: "GET_STATUS"
   - Manual watering: "MANUAL_WATER"
   - Stop watering: "STOP_WATER"
4. **Real-time Data**: Automatic updates every 10 seconds to phone
5. **Connection Status**: Monitor Bluetooth connection in system status

## Troubleshooting

### Common Issues
- **Pump not running**: Check relay connections and power supply
- **False readings**: Clean sensor contacts, check wiring
- **System not responding**: Verify Arduino connections and upload

### Serial Monitor Output
Monitor system status through Serial Monitor at 9600 baud rate.

## Safety Notes

⚠️ **Important Safety Guidelines**
- Use proper relay for pump switching
- Ensure water-resistant connections
- Don't leave system unattended for extended periods
- Check water reservoir regularly
- Use appropriate power supply for pump

## License

This project is for educational purposes (CENG 3264 coursework).

## Author

Created for CENG 3264 - Embedded Systems Course