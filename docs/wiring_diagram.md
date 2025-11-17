# Wiring Diagram

## Pin Connections

### SparkFun RedBoard (DEV-13975) Connections

```
RedBoard Pin   Component                    Connection Details
------------   ---------                    ------------------
A0             Moisture Sensor (3.3V)      Analog output (AO pin)
A1             Moisture Sensor (5V)         Analog output (AO pin)
Digital Pin 7  DRV8871 H-Bridge (IN1)      Motor control input 1
Digital Pin 8  DRV8871 H-Bridge (IN2)      Motor control input 2
Digital Pin 4  nRF52840 (TX)               SoftwareSerial TX (optional)
Digital Pin 5  nRF52840 (RX)               SoftwareSerial RX (optional)
Digital Pin 13 LED                          Built-in LED (or external)
Digital Pin 2  Push Button                  One side to pin, other to GND
3.3V           3.3V Moisture Sensor & nRF52840  Power supply
5V             5V Moisture Sensor & Logic   Power supply (positive)
GND            All Components               Common ground
```

## Detailed Wiring

### Moisture Sensors
#### 3.3V Moisture Sensor (A0)
- **VCC** → RedBoard 3.3V
- **GND** → RedBoard GND  
- **AO** (Analog Out) → RedBoard A0

#### 5V Moisture Sensor (A1)
- **VCC** → RedBoard 5V
- **GND** → RedBoard GND  
- **AO** (Analog Out) → RedBoard A1

### DRV8871 H-Bridge Motor Driver
- **VCC** → 5V (from buck converter or RedBoard)
- **GND** → Common ground
- **IN1** → RedBoard Digital Pin 7
- **IN2** → RedBoard Digital Pin 8
- **OUT1** → DC Motor positive
- **OUT2** → DC Motor negative
- **VM** → Motor power supply (6.5V-45V from buck converter)

### LM2596 Buck Converter
- **VIN+** → External power supply (12V recommended)
- **VIN-** → Ground
- **VOUT+** → Adjust to motor voltage (1.5V-6V for your motor)
- **VOUT-** → Ground
- **Connect to DRV8871 VM pin**

### DC Mini Motor (1.5V-6V, 0.4A-0.55A)
- **Positive** → DRV8871 OUT1
- **Negative** → DRV8871 OUT2
- **Power via H-Bridge from buck converter**

### MOSFET (Optional - for additional switching)
- **Gate** → Control signal (if used)
- **Drain** → Load positive
- **Source** → Load negative
- **5V-36V, 15A continuous (30A max)**

### Nordic nRF52840 Bluetooth Module (Optional)
- **VCC** → RedBoard 3.3V (nRF52840 is 3.3V device)
- **GND** → RedBoard GND
- **TX** → RedBoard Digital Pin 5 (RX on RedBoard)
- **RX** → RedBoard Digital Pin 4 (TX from RedBoard)
- **Enable communication with phone app**
- **Remote threshold setting and monitoring**

### Manual Override Button
- **One terminal** → Arduino Digital Pin 2
- **Other terminal** → Arduino GND
- **Note**: Internal pull-up resistor is used (no external resistor needed)

## Power Supply Architecture

### Power Distribution
1. **Main Power**: 12V external supply → LM2596 Buck Converter
2. **Motor Power**: Buck converter output (adjustable 1.5V-6V) → DRV8871 VM
3. **Logic Power**: RedBoard 5V → DRV8871 VCC, sensors, logic
4. **Sensor Power**: RedBoard 3.3V → 3.3V sensor, RedBoard 5V → 5V sensor

### LM2596 Buck Converter Setup
- **Input**: 12V DC (within 3V-40V range)
- **Output**: Adjust potentiometer for motor voltage (1.5V-6V for your motor)
- **Current**: Capable of handling motor current (0.4A-0.55A)

### DRV8871 H-Bridge Configuration
- **VM (Motor Power)**: From buck converter (6.5V-45V range, set to motor voltage)
- **VCC (Logic Power)**: 5V from RedBoard
- **Current Handling**: Suitable for 0.4A-0.55A motor

## Safety Considerations

⚠️ **Important Safety Notes**
- **Never connect motor directly to RedBoard** - always use H-Bridge
- **Power supply isolation**: Use buck converter for motor power
- **Current protection**: Motor draws 0.4A-0.55A, ensure adequate supply
- **Voltage verification**: Check buck converter output before connecting to H-Bridge
- **Heat dissipation**: H-Bridge and buck converter may generate heat under load
- **Waterproof connections**: Protect all electronics from water exposure
- **Fuse protection**: Consider adding fuse on main power input
- **Ground isolation**: Ensure proper grounding of all components

## Sensor-Specific Information

### DFRobot Moisture Sensor (SEN0114) Characteristics
- **Operating Voltage**: 3.3V - 5V DC
- **Output Voltage**: 0 - 4.2V (analog)
- **Current**: 35mA
- **Probe Material**: Nickel plated
- **Interface**: PH2.0-3P connector
- **Dimensions**: 98mm x 23mm

### SEN0114 Reading Interpretation
- **Dry Soil**: Higher analog values (typically 600-1023)
- **Wet Soil**: Lower analog values (typically 200-400)
- **Water**: Very low values (typically 0-200)
- **Air**: Maximum values (1023)

**Note**: Values are inverted compared to some sensors - lower readings mean MORE moisture!

## Testing Connections

1. **Continuity Test**: Use multimeter to verify all connections
2. **Power Test**: Measure voltages at each component (SEN0114 should show ~5V on VCC)
3. **Sensor Test**: Run calibration sketch to verify sensor readings
4. **Pump Test**: Run pump test sketch to verify relay operation