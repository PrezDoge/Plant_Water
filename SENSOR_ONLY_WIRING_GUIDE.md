# Agricultural Soil Monitoring System - Technical Implementation Guide
## CENG 3264: Embedded Systems Design Project

**PROJECT SCOPE: Wireless Soil Moisture Monitoring and Data Acquisition System**

This document provides comprehensive technical specifications and implementation guidelines for a professional embedded soil monitoring system designed for precision agriculture applications.

## Technical Specifications

### Core System Components

| Component | Part Number | Function | Specifications |
|-----------|-------------|----------|----------------|
| **Primary Controller** | SparkFun RedBoard DEV-15025 | Main processing unit | Arduino Uno compatible, ATmega328P, 16MHz |
| **Sensor Array** | DFRobot SEN0193 (×2) | Soil moisture detection | Capacitive, 3.3-5.5V, analog output |
| **Wireless Module** | SparkFun nRF52840 Mini | Bluetooth Low Energy | Nordic nRF52840, Bluetooth 5.0, 2.4GHz |
| **Development Platform** | Solderless breadboard | Circuit prototyping | 830 tie points, power distribution |
| **Interconnections** | Dupont jumper wires | Signal routing | 22 AWG, male-male connections |
| **Programming Interface** | USB-A to USB-B cable | Firmware upload/debug | Data + power transmission |

### System Architecture Block Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                    AGRICULTURAL SOIL MONITORING SYSTEM                      │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌───────────────┐    ┌──────────────────────────────────────────────────┐ │
│  │   SENSOR      │    │            PROCESSING SUBSYSTEM                  │ │
│  │   ARRAY       │    │                                                  │ │
│  │               │    │  ┌─────────────────┐  ┌─────────────────────────┐ │ │
│  │ ┌───────────┐ │ADC │  │   ATmega328P    │  │     SYSTEM MEMORY       │ │ │
│  │ │ Sensor 1  │─┼────┼──┤     MCU         │  │                         │ │ │
│  │ │ (A0)      │ │CH0 │  │                 │  │ • Program Flash (32KB)  │ │ │
│  │ └───────────┘ │    │  │ • 16MHz Clock   │  │ • SRAM (2KB)           │ │ │
│  │               │    │  │ • 10-bit ADC    │  │ • EEPROM (1KB)         │ │ │
│  │ ┌───────────┐ │ADC │  │ • UART Engine   │  └─────────────────────────┘ │ │
│  │ │ Sensor 2  │─┼────┼──┤                 │                            │ │
│  │ │ (A1)      │ │CH1 │  └─────────────────┘                            │ │
│  │ └───────────┘ │    │                                                  │ │
│  └───────────────┘    └──────────────────────────────────────────────────┘ │
│                                           │                                 │
│                                         UART                               │
│                                           │                                 │
│  ┌─────────────────────────────────────────────────────────────────────────┤ │
│  │              WIRELESS COMMUNICATION SUBSYSTEM                          │ │
│  │                                                                         │ │
│  │  ┌─────────────────┐  ┌─────────────────────────────────────────────┐  │ │
│  │  │   nRF52840      │  │         BLUETOOTH LOW ENERGY                │  │ │
│  │  │   SoC Module    │  │                                             │  │ │
│  │  │                 │  │ • 2.4GHz ISM Band                          │  │ │
│  │  │ • ARM Cortex-M4 │  │ • Bluetooth 5.0 Protocol                   │  │ │
│  │  │ • 64MHz Clock   │  │ • GATT Service Architecture                 │  │ │
│  │  │ • 256KB RAM     │  │ • Mobile Device Connectivity                │  │ │
│  │  │ • 1MB Flash     │  │ • Command/Response Interface                │  │ │
│  │  └─────────────────┘  └─────────────────────────────────────────────┘  │ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
│                                           │                                 │
│                                      BLE Protocol                           │
│                                           │                                 │
│  ┌─────────────────────────────────────────────────────────────────────────┤ │
│  │                    USER INTERFACE LAYER                                │ │
│  │                                                                         │ │
│  │         ┌─────────────────┐              ┌─────────────────────────┐    │ │
│  │         │  MOBILE DEVICE  │              │   DEBUG INTERFACE       │    │ │
│  │         │                 │              │                         │    │ │
│  │         │ • Data Display  │              │ • Serial Monitor        │    │ │
│  │         │ • Configuration │              │ • System Diagnostics    │    │ │
│  │         │ • Alert Receipt │              │ • Parameter Tuning      │    │ │
│  │         │ • Remote Control│              │ • Firmware Upload       │    │ │
│  │         └─────────────────┘              └─────────────────────────┘    │ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Technical Architecture Summary
- **Processing Core**: 8-bit AVR microcontroller (ATmega328P) with 10-bit successive approximation ADC
- **Communication Stack**: UART-based serial interface with Bluetooth Low Energy wireless protocol
- **Power Architecture**: USB-powered (5V) with integrated 3.3V LDO regulation
- **Data Acquisition**: Dual-channel analog front-end with configurable sampling parameters
- **Wireless Implementation**: Nordic nRF52840 SoC providing Bluetooth 5.0 connectivity  

## Circuit Implementation

### Primary Controller Interface (SparkFun RedBoard DEV-15025)

| Pin | Signal Name | Connection | Function |
|-----|-------------|------------|----------|
| **A0** | ADC_CH0 | Soil Sensor 1 Output | Analog input channel 0 |
| **A1** | ADC_CH1 | Soil Sensor 2 Output | Analog input channel 1 |
| **D4** | UART_TX | nRF52840 Module RX | SoftwareSerial transmit |
| **D5** | UART_RX | nRF52840 Module TX | SoftwareSerial receive |
| **D13** | STATUS_LED | Built-in LED | System status indicator |
| **5V** | VCC_MAIN | Power distribution rail | Primary power supply |
| **3.3V** | VCC_AUX | 3.3V distribution rail | Secondary power supply |
| **GND** | GROUND | Common ground rail | System ground reference |

### Power Distribution Network
```
RedBoard 5V Output → Breadboard positive rail → Sensor VCC inputs
RedBoard GND Output → Breadboard ground rail → All ground connections
RedBoard 3.3V Output → Alternative sensor power (if required)
```

### Sensor Interface Specifications

#### DFRobot SEN0193 Capacitive Soil Moisture Sensors

| Parameter | Sensor 1 (Primary) | Sensor 2 (Secondary) |
|-----------|---------------------|----------------------|
| **Power Input** | VCC → 5V rail | VCC → 5V rail |
| **Ground Reference** | GND → Ground rail | GND → Ground rail |
| **Signal Output** | AOUT → RedBoard A0 | AOUT → RedBoard A1 |
| **Operating Voltage** | 3.3V - 5.5V DC | 3.3V - 5.5V DC |
| **Output Range** | 0V - VCC (analog) | 0V - VCC (analog) |
| **Response Time** | <1 second | <1 second |

#### SparkFun nRF52840 Mini Wireless Module

| Parameter | Connection | Specification |
|-----------|------------|---------------|
| **Power Input** | VIN → 5V rail | 3.3V - 16V input range |
| **Ground Reference** | GND → Ground rail | Common system ground |
| **UART Receive** | RX → RedBoard D4 | 3.3V logic level |
| **UART Transmit** | TX → RedBoard D5 | 3.3V logic level |
| **Communication Protocol** | Bluetooth Low Energy | 2.4GHz ISM band |
| **Data Rate** | 9600 baud UART | Configurable transmission |

## Physical Implementation Layout

### Breadboard Configuration Matrix

```
Power Distribution (Top Rails):
    (+) ←→ RedBoard 5V output
    (-) ←→ RedBoard GND output
   (3V) ←→ RedBoard 3.3V output (auxiliary)

Component Placement Grid:
    Row A: Soil Moisture Sensor 1
           [VCC]→(+5V)  [GND]→(-)  [AOUT]→(A0)
    
    Row B: Soil Moisture Sensor 2
           [VCC]→(+5V)  [GND]→(-)  [AOUT]→(A1)
    
    Row C: nRF52840 Bluetooth Module
           [VIN]→(+5V)  [GND]→(-)  [RX]→(D4)  [TX]→(D5)

Inter-Component Signal Routing:
    RedBoard D4  ←→  nRF52840 RX   (MCU transmit to BLE)
    RedBoard D5  ←→  nRF52840 TX   (BLE transmit to MCU)
    RedBoard A0  ←→  Sensor 1 OUT  (Primary moisture signal)
    RedBoard A1  ←→  Sensor 2 OUT  (Secondary moisture signal)
```

## Implementation Procedures

### Phase 1: Hardware Assembly Protocol
1. **Controller Mounting**: Secure RedBoard DEV-15025 to breadboard or development platform
2. **Power Infrastructure**: Establish 5V and GND distribution rails from controller to breadboard
3. **Sensor Integration**: Install soil moisture sensors per connection matrix specifications
4. **Wireless Module Installation**: Mount nRF52840 module with proper UART connections
5. **Connection Verification**: Perform continuity testing on all signal paths before system power-up

### Phase 2: Firmware Deployment
1. **Code Upload**: Deploy `plant_monitor_sensors_only.ino` via Arduino IDE or compatible toolchain
2. **Serial Interface**: Configure serial monitor at 9600 baud for system diagnostics
3. **ADC Calibration**: Verify analog-to-digital converter readings on channels A0 and A1
4. **Wireless Validation**: Establish Bluetooth pairing with mobile device for data verification
5. **System Integration Testing**: Execute full-system operational validation protocol

### Phase 3: Sensor Calibration Matrix

#### Capacitive Soil Moisture Sensor Response Characteristics

| Soil Condition | ADC Reading Range | Volumetric Water Content | System Response |
|----------------|-------------------|------------------------|-----------------|
| **Saturated** | 100-250 | 90-100% VWC | Optimal moisture detected |
| **Moist** | 200-400 | 60-90% VWC | Adequate moisture level |
| **Dry** | 400-600 | 20-60% VWC | Approaching threshold |
| **Arid** | 600-900 | 0-20% VWC | Alert condition triggered |

### Phase 4: System Validation Protocol
1. **Baseline Measurement**: Deploy sensors in controlled dry medium, verify high ADC readings
2. **Moisture Response**: Introduce calibrated water content, verify inverse correlation of readings
3. **Wireless Connectivity**: Establish Bluetooth Low Energy connection with mobile monitoring device
4. **Alert System Validation**: Simulate threshold violation conditions, verify notification generation
5. **Remote Configuration**: Test threshold parameter modification via wireless interface
6. **Data Integrity**: Verify timestamp correlation and measurement consistency across extended operation

## System Functional Requirements

### FR-001: Data Acquisition Subsystem
- **REQ-001.1**: Continuous dual-channel soil moisture monitoring with 10-bit ADC resolution
- **REQ-001.2**: Configurable sampling intervals from 1-60 seconds (default: 5 seconds)
- **REQ-001.3**: Timestamp correlation for all sensor measurements using system clock
- **REQ-001.4**: Statistical data aggregation with measurement counters and trend analysis
- **REQ-001.5**: Sensor calibration coefficients for volumetric water content conversion

### FR-002: Wireless Communication Interface
- **REQ-002.1**: Bluetooth Low Energy protocol implementation for mobile device connectivity
- **REQ-002.2**: Real-time sensor data streaming with configurable transmission intervals
- **REQ-002.3**: Bidirectional command interface for remote system configuration
- **REQ-002.4**: Dynamic device identification with embedded status information
- **REQ-002.5**: Error handling and connection recovery mechanisms

### FR-003: Alert and Notification System
- **REQ-003.1**: Configurable threshold-based alert generation for moisture deficit conditions
- **REQ-003.2**: Multi-modal notification delivery (visual LED, wireless transmission, serial debug)
- **REQ-003.3**: Alert prioritization and escalation protocols for critical conditions
- **REQ-003.4**: User-configurable notification preferences via wireless interface
- **REQ-003.5**: Alert acknowledgment and reset capabilities

## System Performance Specifications

### Quantitative Performance Metrics
- **ADC Resolution**: 10-bit (1024 quantization levels)
- **Sampling Frequency**: 0.2 Hz (5-second intervals)
- **Measurement Accuracy**: ±2% full-scale error
- **Wireless Range**: 10+ meters line-of-sight
- **Power Consumption**: <500mA @ 5V DC
- **Operating Temperature**: 0°C to +50°C
- **Data Retention**: Non-volatile configuration storage
- **System Uptime**: >99% availability over 7-day period

**Engineering Deliverable Classification: Embedded Monitoring System - Data Acquisition Focus**

---

## Appendix A: Technical Reference Documentation

### A.1 Pin Assignment Matrix

| MCU Pin | Function | Signal Name | Direction | Electrical Specs | Connection |
|---------|----------|-------------|-----------|------------------|------------|
| **PC0 (A0)** | ADC Channel 0 | SOIL_SENS_1 | Input | 0-5V, 10-bit resolution | Sensor 1 analog output |
| **PC1 (A1)** | ADC Channel 1 | SOIL_SENS_2 | Input | 0-5V, 10-bit resolution | Sensor 2 analog output |
| **PD4 (D4)** | UART TX | BLE_UART_TX | Output | 5V CMOS logic | nRF52840 RX input |
| **PD5 (D5)** | UART RX | BLE_UART_RX | Input | 5V CMOS logic | nRF52840 TX output |
| **PB5 (D13)** | GPIO Output | STATUS_LED | Output | 5V CMOS, 20mA sink | Built-in LED cathode |
| **VCC** | Power Supply | VDD_5V | Power | +5.0V ±5% | System power rail |
| **GND** | Ground | VSS | Ground | 0V reference | System ground plane |

### A.2 Communication Protocol Specification

#### UART Interface Parameters
- **Baud Rate**: 9600 bits per second
- **Data Format**: 8 data bits, no parity, 1 stop bit (8N1)
- **Flow Control**: None (software controlled)
- **Voltage Levels**: 0V (logic low), 5V (logic high)

#### Bluetooth Low Energy Profile
- **Protocol Version**: Bluetooth 5.0 specification
- **Operating Frequency**: 2.400-2.485 GHz ISM band
- **Modulation**: GFSK (Gaussian Frequency Shift Keying)
- **Data Rate**: 1 Mbps (base rate), up to 2 Mbps (enhanced)
- **Range**: 10+ meters (Class 2 device)

### A.3 Sensor Specifications and Calibration Data

#### DFRobot SEN0193 Capacitive Soil Moisture Sensor
- **Operating Voltage**: 3.3V - 5.5V DC
- **Operating Current**: 5mA (typical)
- **Output Voltage**: 0V - VCC (proportional to moisture)
- **Response Time**: <1 second (90% final value)
- **Operating Temperature**: -40°C to +85°C
- **Probe Dimensions**: 98mm × 23mm × 8mm

#### Moisture Correlation Function
```
VWC% = 100 × (1 - (ADC_Value - ADC_Min) / (ADC_Max - ADC_Min))

Where:
- VWC% = Volumetric Water Content percentage
- ADC_Value = Current sensor reading (0-1023)
- ADC_Min = Sensor reading in water (typically 100-200)
- ADC_Max = Sensor reading in dry air (typically 800-900)
```

---
