# Plant Watering System - Complete Wiring Guide

## Hardware Overview

### Current Wiring Status
The basic sensor connections to the SparkFun RedBoard are established:

**Moisture Sensors:**
- Left Sensor (3.3V) → RedBoard A0 (Analog Pin)
- Right Sensor (5V) → RedBoard A1 (Analog Pin)  
- Power connections for both sensors

## Complete System Wiring Diagram

### All Components Pin Mapping

| Component | RedBoard Pin | Wire Color | Notes |
|-----------|-------------|------------|-------|
| **Moisture Sensors** |
| 3.3V Moisture Sensor (Signal) | A0 | Red | Analog input |
| 5V Moisture Sensor (Signal) | A1 | Blue | Analog input |
| 3.3V Sensor VCC | 3.3V | Red | Power rail |
| 5V Sensor VCC | 5V | Red | Power rail |
| Both Sensor GND | GND | Black | Common ground |
| **Motor Control System** |
| DRV8871 IN1 | Pin 7 | Yellow | Motor direction control 1 |
| DRV8871 IN2 | Pin 8 | Green | Motor direction control 2 |
| DRV8871 VM (Motor Power) | External 12V | Red | High current motor supply |
| DRV8871 VCC (Logic Power) | 5V | Red | Logic level supply |
| DRV8871 GND | GND | Black | Common ground |
| Motor Positive | DRV8871 OUT1 | Red | Motor terminal |
| Motor Negative | DRV8871 OUT2 | Black | Motor terminal |
| **Bluetooth Communication** |
| SparkFun nRF52840 TX | Pin 5 | Orange | Arduino RX (SoftwareSerial) |
| SparkFun nRF52840 RX | Pin 4 | Purple | Arduino TX (SoftwareSerial) |
| SparkFun nRF52840 VCC | 3.3V | Red | 3.3V power only |
| SparkFun nRF52840 GND | GND | Black | Common ground |
| **Power System** |
| LM2596 IN+ | 12V DC Supply | Red | Input voltage |
| LM2596 IN- | 12V DC Supply | Black | Input ground |
| LM2596 OUT+ | RedBoard VIN | Red | Regulated 5V output |
| LM2596 OUT- | RedBoard GND | Black | Output ground |
| **Status Indicators** |
| Built-in LED | Pin 13 | - | System status indicator |

### Bluetooth Module Pin Details
The SparkFun Pro nRF52840 Mini connects to the RedBoard using SoftwareSerial communication:
- **TX (nRF52840) → Pin 5 (RedBoard)**: Data transmission from Bluetooth module
- **RX (nRF52840) → Pin 4 (RedBoard)**: Data reception to Bluetooth module  
- **VCC → 3.3V Rail**: Power supply (critical: never use 5V)
- **GND → Common Ground**: Shared ground connection



## Power Supply Considerations

### **Voltage Requirements**
- **RedBoard**: 5V via USB or barrel jack
- **3.3V Sensor**: 3.3V from RedBoard 3.3V pin
- **5V Sensor**: 5V from RedBoard 5V pin
- **SparkFun nRF52840**: 3.3V (do not use 5V!)
- **DRV8871**: 6.5V-45V (use external power for motor)
- **DC Motor**: 1.5V-6V (through H-bridge)

### **Power Supply Setup**
```
Main Power → 12V DC Adapter
12V → LM2596 Buck Converter → 5V → RedBoard
5V → RedBoard 3.3V regulator → 3.3V components
12V → DRV8871 motor driver → DC Motor
```

## Complete System Wiring

### Breadboard Layout

**Power Rail Configuration:**
- Top rail: +5V (from RedBoard regulated output)
- Bottom rail: GND (common ground for all components)
- Side rail: +3.3V (from RedBoard 3.3V output)

**Component Physical Layout:**
- SparkFun nRF52840 Mini: Left section of breadboard
- DRV8871 H-Bridge: Right section of breadboard  
- Moisture Sensors: Connected via jumper wires to measurement points
- Power regulation components: Separate area for LM2596

**Wire Management Standards:**
- Red wires: All positive power connections (5V, 3.3V, 12V)
- Black wires: All ground connections
- Colored signal wires: Data and control lines (yellow, green, orange, purple)
- Wire labeling: Use tape labels for complex connections to enable troubleshooting

## Assembly Steps

### Step 1: Verify Moisture Sensor Connections
Current sensor setup is correctly implemented according to your existing diagram:
- 3.3V moisture sensor signal → RedBoard pin A0
- 5V moisture sensor signal → RedBoard pin A1
- Power and ground connections established

### Step 2: Install Motor Control System
1. Mount DRV8871 H-Bridge on breadboard
2. Connect control pins: IN1 → Pin 7, IN2 → Pin 8
3. Connect motor power: VM → 12V external, VCC → 5V logic
4. Wire motor terminals to DRV8871 OUT1 and OUT2
5. Establish common ground connections

### Step 3: Integrate Bluetooth Communication
1. Position SparkFun nRF52840 Mini on breadboard
2. Wire communication: TX → Pin 5, RX → Pin 4 (SoftwareSerial)
3. Power connection: VCC → 3.3V rail (critical: do not use 5V)
4. Connect GND to common ground rail

### Step 4: Implement Power Distribution System
1. Connect 12V external power supply to system
2. Install LM2596 buck converter: 12V input → 5V regulated output
3. Route 5V to RedBoard VIN pin for system power
4. Verify 3.3V regulation from RedBoard for nRF52840 Mini
5. Test all voltage levels before connecting components

## Testing Sequence

### 1. Sensor Validation Testing
```cpp
// Upload plant_water_basic.ino to RedBoard
// Open Serial Monitor at 9600 baud
// Verify moisture sensor readings display correctly
// Test both dry and wet conditions for each sensor
```

### 2. Motor Control System Testing  
```cpp
// Upload examples/pump_test.ino to RedBoard
// Verify motor runs in forward direction
// Test motor reverse functionality
// Confirm H-bridge thermal protection
// Check motor current consumption
```

### 3. Bluetooth Communication Testing
```cpp
// Upload plant_water.ino to RedBoard
// Enable phone Bluetooth and scan for devices
// Look for "Plant Monitor - Status" in device list
// Pair with device and test popup interface functionality
// Verify threshold setting and device naming features
```