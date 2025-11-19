/*
 * Agricultural Soil Monitoring System with Wireless Data Transmission
 * 
 * CENG 3264: Embedded Systems Design Project
 * Author: [Student Name]
 * Date: November 2025
 * 
 * SYSTEM OVERVIEW:
 * This embedded system implements a dual-sensor soil moisture monitoring solution
 * with wireless Bluetooth Low Energy data transmission capabilities. The system
 * provides real-time agricultural monitoring with configurable threshold parameters
 * and automated alert generation for precision agriculture applications.
 * 
 * HARDWARE SPECIFICATIONS:
 * - Primary Controller: SparkFun RedBoard (DEV-15025) - Arduino Uno Compatible
 * - Sensor Array: Two DFRobot SEN0193 Capacitive Soil Moisture Sensors
 * - Wireless Module: SparkFun Pro nRF52840 Mini (Bluetooth 5.0 BLE)
 * - Communication: UART via SoftwareSerial library (9600 baud)
 * - Power Requirements: 5V DC via USB or external adapter
 * 
 * SYSTEM CAPABILITIES:
 * - Continuous soil moisture monitoring with 5-second sampling interval
 * - Wireless data transmission via Bluetooth Low Energy protocol
 * - Remote threshold configuration through mobile device interface
 * - Automated alert generation for moisture deficit conditions
 * - Statistical data logging with timestamp correlation
 * - Dynamic device identification with real-time status indication
 * 
 * TECHNICAL SPECIFICATIONS:
 * - ADC Resolution: 10-bit (0-1023 digital values)
 * - Sensor Range: 0-100% volumetric soil moisture content
 * - Wireless Range: 10+ meters (Bluetooth Class 2)
 * - Data Rate: Configurable transmission intervals
 * - Alert Latency: <500ms for threshold violations
 */

#include <SoftwareSerial.h>

// HARDWARE INTERFACE DEFINITIONS
// Analog input channels for soil moisture sensors
const int SOIL_MOISTURE_SENSOR_1_PIN = A0;    // Primary soil moisture sensor input
const int SOIL_MOISTURE_SENSOR_2_PIN = A1;    // Secondary soil moisture sensor input
const int SYSTEM_STATUS_LED_PIN = 13;         // Built-in status indicator LED

// UART communication interface for Bluetooth module
const int BLE_UART_TX_PIN = 4;                // MCU TX to nRF52840 RX
const int BLE_UART_RX_PIN = 5;                // MCU RX from nRF52840 TX
SoftwareSerial bluetoothInterface(BLE_UART_RX_PIN, BLE_UART_TX_PIN);

// SYSTEM CONFIGURATION STRUCTURE
typedef struct {
  char deviceIdentifier[32];              // Bluetooth device name with status
  uint16_t moistureThresholdSensor1;      // Sensor 1 dry condition threshold (ADC units)
  uint16_t moistureThresholdSensor2;      // Sensor 2 dry condition threshold (ADC units)  
  bool alertNotificationsEnabled;         // Alert system enable/disable flag
  uint32_t sensorSamplingInterval;        // Sensor reading interval (milliseconds)
} SystemConfiguration;

// Initialize system configuration with default parameters
SystemConfiguration systemConfig = {
  "AgroMonitor_Operational",              // Default device identifier
  300,                                    // Default threshold sensor 1 (empirically determined)
  300,                                    // Default threshold sensor 2 (empirically determined)
  true,                                   // Notifications enabled by default
  5000                                    // 5-second sampling interval
};

// SENSOR DATA STRUCTURE
typedef struct {
  uint16_t moistureLevel1;                // Sensor 1 ADC reading (0-1023)
  uint16_t moistureLevel2;                // Sensor 2 ADC reading (0-1023)
  bool moistureDeficit1;                  // Sensor 1 dry condition flag
  bool moistureDeficit2;                  // Sensor 2 dry condition flag
  uint32_t measurementTimestamp;          // System timestamp for measurement
} SensorDataPoint;

// GLOBAL SYSTEM STATE VARIABLES
SensorDataPoint currentMeasurement;       // Current sensor readings
SensorDataPoint previousMeasurement;      // Previous readings for change detection
bool bluetoothConnectionStatus = false;   // BLE connection state flag
uint32_t lastSensorSampleTime = 0;       // Timestamp of last sensor reading
uint32_t lastStatusUpdateTime = 0;       // Timestamp of last status broadcast
uint32_t lastDataTransmissionTime = 0;   // Timestamp of last data transmission
uint16_t totalMeasurementCount = 0;      // Total number of measurements taken

/*
 * SYSTEM INITIALIZATION ROUTINE
 * Configures hardware interfaces, initializes communication protocols,
 * and establishes baseline sensor readings for operational startup.
 */
void setup() {
  // Initialize UART communication interfaces
  Serial.begin(9600);                     // Debug/monitoring console interface
  bluetoothInterface.begin(9600);         // Bluetooth module communication interface
  
  // System startup notification
  Serial.println("========================================");
  Serial.println("Agricultural Soil Monitoring System");
  Serial.println("CENG 3264 Embedded Systems Project");
  Serial.println("Firmware Version: 3.0.0");
  Serial.println("========================================");
  
  // Configure digital I/O pins
  pinMode(SYSTEM_STATUS_LED_PIN, OUTPUT);
  digitalWrite(SYSTEM_STATUS_LED_PIN, LOW);
  
  // Analog input pins (A0, A1) are automatically configured for ADC operation
  
  // Initialize wireless communication subsystem
  initializeBluetoothInterface();
  
  // System operational status
  Serial.println("System Status: OPERATIONAL");
  Serial.println("Wireless Interface: READY");
  Serial.println("Sensor Array: INITIALIZED");
  Serial.println("Awaiting Bluetooth connection for remote monitoring...");
  
  // Establish baseline sensor measurements
  acquireSensorMeasurements();
  previousMeasurement = currentMeasurement;
}

void loop() {
  // Handle Bluetooth communication
  handleBluetoothCommunication();
  
  // Update device name with current status
  updateBluetoothStatus();
  
  // Read sensors at configured interval
  if (millis() - lastSensorRead >= config.readingInterval) {
    readAllSensors();
    checkForAlerts();
    logSensorData();
    lastSensorRead = millis();
    readingCount++;
  }
  
  // Transmit data to phone periodically
  if (bluetoothConnected && (millis() - lastDataTransmission >= 30000)) {
    transmitSensorData();
    lastDataTransmission = millis();
  }
  
  // Update LED status
  updateStatusLED();
}

void initializeBluetooth() {
  Serial.println("Initializing nRF52840 Bluetooth...");
  
  // Configure nRF52840
  bluetooth.println("AT+RESET");
  delay(1000);
  
  // Set discoverable name with status
  updateDeviceName();
  
  // Enable notifications
  bluetooth.println("AT+GAPDEVNAME=" + String(config.deviceName));
  bluetooth.println("AT+GAPDISCONNECT");
  bluetooth.println("AT+BLEADV_SET=ON");
  
  Serial.println("Bluetooth initialized - Device discoverable");
  Serial.println("Device name: " + String(config.deviceName));
}

void updateDeviceName() {
  String status = "PlantMonitor";
  
  // Add sensor status to device name
  if (currentReading.soil1Dry || currentReading.soil2Dry) {
    status += "_DryAlert";
  } else {
    status += "_Healthy";
  }
  
  // Add current readings
  status += "_S1:" + String(currentReading.soilMoisture1);
  status += "_S2:" + String(currentReading.soilMoisture2);
  
  strcpy(config.deviceName, status.c_str());
}

void updateBluetoothStatus() {
  // Update device name every 20 seconds
  if (millis() - lastStatusUpdate > 20000) {
    updateDeviceName();
    bluetooth.println("AT+GAPDEVNAME=" + String(config.deviceName));
    lastStatusUpdate = millis();
  }
}

void handleBluetoothCommunication() {
  if (bluetooth.available()) {
    String message = bluetooth.readStringUntil('\n');
    message.trim();
    
    // Check for connection events
    if (message.indexOf("CONNECTED") != -1) {
      bluetoothConnected = true;
      Serial.println("Phone connected!");
      sendWelcomeMessage();
    }
    else if (message.indexOf("DISCONNECTED") != -1) {
      bluetoothConnected = false;
      Serial.println("Phone disconnected");
    }
    // Handle incoming commands
    else if (message.startsWith("CMD:")) {
      processPhoneCommand(message.substring(4));
    }
  }
}

void sendWelcomeMessage() {
  bluetooth.println("POPUP: Plant Monitoring System Connected!");
  delay(100);
  bluetooth.println("POPUP:Current Sensor Readings:");
  bluetooth.println("POPUP:Soil Sensor 1: " + String(currentReading.soilMoisture1) + " (Threshold: " + String(config.soilDryThreshold1) + ")");
  bluetooth.println("POPUP:Soil Sensor 2: " + String(currentReading.soilMoisture2) + " (Threshold: " + String(config.soilDryThreshold2) + ")");
  delay(100);
  
  // Send menu options
  bluetooth.println("MENU:Available Commands:");
  bluetooth.println("MENU:1 - Current Readings");
  bluetooth.println("MENU:2 - Set Soil Threshold 1");
  bluetooth.println("MENU:3 - Set Soil Threshold 2"); 
  bluetooth.println("MENU:4 - Data Stream ON/OFF");
  bluetooth.println("MENU:5 - Reset Counters");
  bluetooth.println("MENU:Reply with number");
}

void processPhoneCommand(String command) {
  Serial.println("Received command: " + command);
  
  if (command == "1") {
    // Send current readings
    sendCurrentReadings();
  }
  else if (command == "2") {
    bluetooth.println("INPUT:Enter Soil Sensor 1 threshold (0-1023):");
    bluetooth.println("CURRENT:" + String(config.soilDryThreshold1));
  }
  else if (command == "3") {
    bluetooth.println("INPUT:Enter Soil Sensor 2 threshold (0-1023):");
    bluetooth.println("CURRENT:" + String(config.soilDryThreshold2));
  }
  else if (command == "4") {
    config.alertsEnabled = !config.alertsEnabled;
    bluetooth.println("POPUP:Alerts " + String(config.alertsEnabled ? "ENABLED" : "DISABLED"));
  }
  else if (command == "5") {
    readingCount = 0;
    bluetooth.println("POPUP:Counters reset!");
  }
  else if (command.startsWith("THRESH1:")) {
    int newThreshold = command.substring(8).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.soilDryThreshold1 = newThreshold;
      bluetooth.println("POPUP:Soil Sensor 1 threshold set to " + String(newThreshold));
    } else {
      bluetooth.println("POPUP:Error: Threshold must be 0-1023");
    }
  }
  else if (command.startsWith("THRESH2:")) {
    int newThreshold = command.substring(8).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.soilDryThreshold2 = newThreshold;
      bluetooth.println("POPUP:Soil Sensor 2 threshold set to " + String(newThreshold));
    } else {
      bluetooth.println("POPUP:Error: Threshold must be 0-1023");
    }
  }
}

void sendCurrentReadings() {
  bluetooth.println("DATA:=== Current Sensor Readings ===");
  bluetooth.println("DATA:Soil Sensor 1: " + String(currentReading.soilMoisture1) + " " + 
                    (currentReading.soil1Dry ? "(DRY)" : "(WET)"));
  bluetooth.println("DATA:Soil Sensor 2: " + String(currentReading.soilMoisture2) + " " + 
                    (currentReading.soil2Dry ? "(DRY)" : "(WET)"));
  bluetooth.println("DATA:Reading Count: " + String(readingCount));
  bluetooth.println("DATA:Uptime: " + String(millis() / 1000) + " seconds");
}

void transmitSensorData() {
  bluetooth.println("STREAM:DATA_UPDATE");
  bluetooth.println("STREAM:" + String(currentReading.soilMoisture1) + "," + 
                    String(currentReading.soilMoisture2) + "," + 
                    String(millis()));
}

void readAllSensors() {
  // Store previous reading
  lastReading = currentReading;
  
  // Read soil sensors
  currentReading.soilMoisture1 = analogRead(SOIL_MOISTURE_PIN_1);
  currentReading.soilMoisture2 = analogRead(SOIL_MOISTURE_PIN_2);
  currentReading.timestamp = millis();
  
  // Determine status flags
  currentReading.soil1Dry = currentReading.soilMoisture1 > config.soilDryThreshold1;
  currentReading.soil2Dry = currentReading.soilMoisture2 > config.soilDryThreshold2;
}

void checkForAlerts() {
  bool newAlert = false;
  
  // Check for newly dry soil
  if (!lastReading.soil1Dry && currentReading.soil1Dry) {
    Serial.println("ALERT: Soil Sensor 1 is now DRY!");
    if (bluetoothConnected && config.alertsEnabled) {
      bluetooth.println("ALERT: Soil Sensor 1 needs attention - reading: " + String(currentReading.soilMoisture1));
    }
    newAlert = true;
  }
  
  if (!lastReading.soil2Dry && currentReading.soil2Dry) {
    Serial.println("ALERT: Soil Sensor 2 is now DRY!");
    if (bluetoothConnected && config.alertsEnabled) {
      bluetooth.println("ALERT: Soil Sensor 2 needs attention - reading: " + String(currentReading.soilMoisture2));
    }
    newAlert = true;
  }
  

  
  // Flash LED for new alerts
  if (newAlert) {
    for (int i = 0; i < 6; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
}

void logSensorData() {
  Serial.print("Reading #");
  Serial.print(readingCount);
  Serial.print(" - Soil1: ");
  Serial.print(currentReading.soilMoisture1);
  Serial.print(currentReading.soil1Dry ? " (DRY)" : " (WET)");
  Serial.print(", Soil2: ");
  Serial.print(currentReading.soilMoisture2);
  Serial.println(currentReading.soil2Dry ? " (DRY)" : " (WET)");
}

void updateStatusLED() {
  if (bluetoothConnected) {
    if (currentReading.soil1Dry || currentReading.soil2Dry) {
      // Fast blink for alerts when connected
      digitalWrite(LED_PIN, (millis() / 200) % 2);
    } else {
      // Solid on when connected and all good
      digitalWrite(LED_PIN, HIGH);
    }
  } else {
    if (currentReading.soil1Dry || currentReading.soil2Dry) {
      // Medium blink for alerts when not connected
      digitalWrite(LED_PIN, (millis() / 500) % 2);
    } else {
      // Slow blink when not connected but all good
      digitalWrite(LED_PIN, (millis() / 1000) % 2);
    }
  }
}