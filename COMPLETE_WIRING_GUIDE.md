# Plant Watering System - Complete Wiring Guide

## 📋 Hardware Overview

### **Current Wiring (From Your Diagram)**
Your diagram shows the basic sensor connections to the SparkFun RedBoard:

**Moisture Sensors:**
- **Left Sensor (3.3V)** → RedBoard A0 (Analog Pin)
- **Right Sensor (5V)** → RedBoard A1 (Analog Pin)
- **Power connections** for both sensors

## 🔌 Complete Wiring Diagram

### **All Components Pin Mapping**

| Component | RedBoard Pin | Wire Color | Notes |
|-----------|-------------|------------|-------|
| **Moisture Sensors** |
| 3.3V Moisture Sensor (Signal) | A0 | Red | From your diagram |
| 5V Moisture Sensor (Signal) | A1 | Blue | From your diagram |
| 3.3V Sensor Power | 3.3V | - | Power rail |
| 5V Sensor Power | 5V | - | Power rail |
| Sensor Grounds | GND | Black | Common ground |
| **Motor Control (H-Bridge)** |
| DRV8871 IN1 | Pin 7 | Yellow | Motor direction 1 |
| DRV8871 IN2 | Pin 8 | Green | Motor direction 2 |
| DRV8871 VCC | 5V | Red | Power supply |
| DRV8871 GND | GND | Black | Ground |
| **Bluetooth Module (nRF52840)** |
| nRF52840 TX | Pin 4 | Orange | Arduino RX |
| nRF52840 RX | Pin 5 | Purple | Arduino TX |
| nRF52840 VCC | 3.3V | Red | 3.3V power |
| nRF52840 GND | GND | Black | Ground |
| **Status Indicator** |
| Status LED | Pin 13 | - | Built-in LED |

## 🔧 Additional Components Needed

### **Motor Control System**
```
RedBoard Pin 7 → DRV8871 IN1
RedBoard Pin 8 → DRV8871 IN2
DRV8871 OUT1 → DC Motor +
DRV8871 OUT2 → DC Motor -
DRV8871 VCC → 5V (or higher for motor)
DRV8871 GND → Common Ground
```

### **Bluetooth Module (nRF52840)**
```
RedBoard Pin 4 → nRF52840 RX
RedBoard Pin 5 → nRF52840 TX  
RedBoard 3.3V → nRF52840 VCC
RedBoard GND → nRF52840 GND
```



## ⚡ Power Supply Considerations

### **Voltage Requirements**
- **RedBoard**: 5V via USB or barrel jack
- **3.3V Sensor**: 3.3V from RedBoard 3.3V pin
- **5V Sensor**: 5V from RedBoard 5V pin
- **nRF52840**: 3.3V (do not use 5V!)
- **DRV8871**: 6.5V-45V (use external power for motor)
- **DC Motor**: 1.5V-6V (through H-bridge)

### **Power Supply Setup**
```
Main Power → 12V DC Adapter
12V → LM2596 Buck Converter → 5V → RedBoard
5V → RedBoard 3.3V regulator → 3.3V components
12V → DRV8871 motor driver → DC Motor
```

## 🔗 Complete System Wiring

### **Breadboard Layout Suggestion**
```
Power Rails:
- Top rail: 5V (from RedBoard)
- Bottom rail: GND (common ground)
- Side rail: 3.3V (from RedBoard)

Component Placement:
- nRF52840: Left side of breadboard
- DRV8871: Right side of breadboard
- Sensors: Connect via jumper wires to soil
```

### **Wire Management**
- **Red wires**: Power (5V, 3.3V)
- **Black wires**: Ground connections
- **Colored wires**: Signal/data lines
- **Label connections**: For easy troubleshooting

## 🛠️ Assembly Steps

### **Step 1: Basic Connections (Your Current Setup)**
✅ You already have the moisture sensors connected correctly!

### **Step 2: Add Motor Control**
1. Connect DRV8871 H-bridge to pins 7 and 8
2. Connect motor to H-bridge outputs
3. Provide appropriate power supply to H-bridge

### **Step 3: Add Bluetooth Module**
1. Connect nRF52840 to pins 4 and 5
2. **Important**: Use 3.3V power, not 5V!
3. Connect common ground

### **Step 4: Power Supply**
1. Use external 12V supply for motor power
2. Buck converter to provide regulated 5V
3. RedBoard provides regulated 3.3V

## 🧪 Testing Sequence

### **1. Test Sensors First** (You can do this now!)
```cpp
// Upload plant_water_basic.ino
// Open Serial Monitor
// Check sensor readings
```

### **2. Test Motor Control**
```cpp
// Upload examples/pump_test.ino
// Verify motor runs forward/reverse
// Check H-bridge connections
```

### **3. Test Bluetooth**
```cpp
// Upload plant_water.ino
// Pair with phone
// Test popup interface
```

## ⚠️ Safety Notes

### **Electrical Safety**
- **Double-check voltage levels** before connecting
- **nRF52840 is 3.3V only** - 5V will damage it!
- **Isolate motor power** from logic power
- **Use fuses** on high-current connections

### **Water Safety**
- **Keep electronics away from water**
- **Use waterproof connectors** for sensors
- **Test system before leaving unattended**

## 📊 Troubleshooting

### **Sensors Not Reading**
- Check A0 and A1 connections
- Verify sensor power (3.3V and 5V)
- Test with multimeter

### **Motor Not Working**
- Check H-bridge connections (pins 7, 8)
- Verify motor power supply
- Test H-bridge with LED first

### **Bluetooth Not Connecting**
- Verify 3.3V power to nRF52840
- Check TX/RX connections (crossed correctly)
- Ensure common ground connection

---

**Your sensor wiring looks perfect! Now just add the missing components following this guide. 🌱**