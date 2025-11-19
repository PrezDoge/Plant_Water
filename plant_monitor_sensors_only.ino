/*
 * ========================================================================
 * SOIL MOISTURE MONITORING SYSTEM - SENSOR ONLY MODE
 * ========================================================================
 * 
 * PROJECT: CENG 3264 Embedded Systems Design
 * BOARD: Arduino Uno (Select "Arduino Uno" in Arduino IDE Tools > Board)
 * PURPOSE: Professional soil monitoring with Bluetooth data transmission
 * 
 * HARDWARE REQUIREMENTS:
 * - Arduino Uno or compatible (SparkFun RedBoard DEV-15025)
 * - 2x DFRobot SEN0193 Capacitive Soil Moisture Sensors
 * - SparkFun Pro nRF52840 Mini (Bluetooth module)
 * - Jumper wires and breadboard
 * 
 * ARDUINO IDE SETUP:
 * 1. Select Tools > Board > "Arduino Uno"
 * 2. Select correct COM port under Tools > Port
 * 3. Verify sketch compiles without errors
 * 4. Upload to board
 * 
 * PIN CONNECTIONS:
 * - Soil Sensor 1 → A0 (analog)
 * - Soil Sensor 2 → A1 (analog) 
 * - nRF52840 TX → Pin 4 (digital)
 * - nRF52840 RX → Pin 5 (digital)
 * - Status LED → Pin 13 (built-in)
 * 
 * FEATURES:
 * - Dual soil moisture monitoring
 * - Bluetooth Low Energy data transmission
 * - Real-time threshold alerts
 * - Remote configuration via mobile device
 * - Professional data logging
 * 
 * ========================================================================
 */

#include <SoftwareSerial.h>

// ========================================================================
// HARDWARE PIN DEFINITIONS
// ========================================================================
const int SOIL_SENSOR_1_PIN = A0;        // Primary soil moisture sensor
const int SOIL_SENSOR_2_PIN = A1;        // Secondary soil moisture sensor  
const int STATUS_LED_PIN = 13;           // Built-in Arduino LED
const int BLUETOOTH_TX_PIN = 4;          // To nRF52840 RX
const int BLUETOOTH_RX_PIN = 5;          // From nRF52840 TX

// Create Bluetooth communication interface
SoftwareSerial bluetooth(BLUETOOTH_RX_PIN, BLUETOOTH_TX_PIN);

// ========================================================================
// SYSTEM CONFIGURATION PARAMETERS
// ========================================================================
struct SystemConfig {
  char deviceName[40];                    // Bluetooth device identifier
  int soilThreshold1;                     // Sensor 1 dry threshold (0-1023)
  int soilThreshold2;                     // Sensor 2 dry threshold (0-1023)
  bool alertsEnabled;                     // Alert notifications on/off
  unsigned long readingInterval;          // Time between sensor readings (ms)
  bool dataStreamEnabled;                 // Continuous data streaming on/off
};

// Initialize with default values
SystemConfig config = {
  "SoilMonitor_Ready",                    // Default device name
  350,                                    // Default threshold sensor 1
  350,                                    // Default threshold sensor 2
  true,                                   // Alerts enabled
  5000,                                   // Read sensors every 5 seconds
  false                                   // Data streaming disabled by default
};

// ========================================================================
// SENSOR DATA STRUCTURE
// ========================================================================
struct SensorReading {
  int moisture1;                          // Sensor 1 value (0-1023)
  int moisture2;                          // Sensor 2 value (0-1023)
  bool soil1Dry;                          // Sensor 1 dry status flag
  bool soil2Dry;                          // Sensor 2 dry status flag
  unsigned long timestamp;                // Reading timestamp
};

// ========================================================================
// GLOBAL SYSTEM VARIABLES
// ========================================================================
SensorReading currentReading;             // Current sensor values
SensorReading previousReading;            // Previous reading for comparison
bool bluetoothConnected = false;          // Phone connection status
unsigned long lastSensorRead = 0;        // Last sensor reading time
unsigned long lastStatusUpdate = 0;      // Last device name update
unsigned long lastDataTransmission = 0;  // Last data stream transmission
unsigned long lastHeartbeat = 0;         // Last heartbeat signal
int totalReadings = 0;                    // Total number of readings taken

// ========================================================================
// ARDUINO SETUP FUNCTION
// ========================================================================
void setup() {
  // Initialize serial communications
  Serial.begin(9600);                     // Debug console (Arduino IDE Serial Monitor)
  bluetooth.begin(9600);                  // Bluetooth module communication
  
  // Print startup banner
  Serial.println(F("========================================"));
  Serial.println(F("SOIL MOISTURE MONITORING SYSTEM"));
  Serial.println(F("CENG 3264 - Sensor Only Mode"));
  Serial.println(F("Arduino Uno Compatible"));
  Serial.println(F("========================================"));
  
  // Configure hardware pins
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);
  
  // Analog pins A0, A1 are automatically configured for input
  
  // Initialize Bluetooth module
  initializeBluetooth();
  
  // Take initial sensor reading
  readSensors();
  previousReading = currentReading;
  
  // System ready
  Serial.println(F("System Status: OPERATIONAL"));
  Serial.println(F("Bluetooth: READY FOR CONNECTION"));
  Serial.println(F("Sensors: INITIALIZED"));
  Serial.println();
  Serial.println(F("Connect via Bluetooth to configure and monitor"));
  Serial.print(F("Device Name: "));
  Serial.println(config.deviceName);
  Serial.println();
  
  // Flash LED to indicate ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(200);
  }
}

// ========================================================================
// ARDUINO MAIN LOOP FUNCTION  
// ========================================================================
void loop() {
  // Handle incoming Bluetooth messages
  handleBluetoothCommunication();
  
  // Update device name with current status periodically
  updateDeviceStatus();
  
  // Read sensors at configured interval
  if (millis() - lastSensorRead >= config.readingInterval) {
    readSensors();
    checkForAlerts();
    logSensorData();
    lastSensorRead = millis();
    totalReadings++;
  }
  
  // Send data stream if enabled and connected
  if (bluetoothConnected && config.dataStreamEnabled && 
      (millis() - lastDataTransmission >= 10000)) {
    transmitDataStream();
    lastDataTransmission = millis();
  }
  
  // Send periodic heartbeat when connected
  if (bluetoothConnected && (millis() - lastHeartbeat >= 60000)) {
    bluetooth.println(F("HEARTBEAT:System operational"));
    lastHeartbeat = millis();
  }
  
  // Update status LED
  updateStatusLED();
  
  // Small delay for system stability
  delay(100);
}

// ========================================================================
// BLUETOOTH INITIALIZATION
// ========================================================================
void initializeBluetooth() {
  Serial.println(F("Initializing Bluetooth module..."));
  
  // Reset and configure nRF52840
  bluetooth.println(F("AT+RESET"));
  delay(1000);
  
  // Set device name
  updateDeviceName();
  
  // Configure advertising
  bluetooth.print(F("AT+GAPDEVNAME="));
  bluetooth.println(config.deviceName);
  bluetooth.println(F("AT+GAPDISCONNECT"));
  bluetooth.println(F("AT+BLEADV_SET=ON"));
  
  Serial.println(F("Bluetooth initialized successfully"));
  Serial.print(F("Device name: "));
  Serial.println(config.deviceName);
}

// ========================================================================
// DEVICE NAME UPDATE WITH SENSOR STATUS
// ========================================================================
void updateDeviceName() {
  // Create device name string manually to avoid String concatenation issues
  strcpy(config.deviceName, "SoilMonitor");
  
  // Add current sensor status
  if (currentReading.soil1Dry || currentReading.soil2Dry) {
    strcat(config.deviceName, "_ALERT");
  } else {
    strcat(config.deviceName, "_OK");
  }
  
  // Add sensor readings (limited by device name length)
  char tempStr[10];
  strcat(config.deviceName, "_S1:");
  itoa(currentReading.moisture1, tempStr, 10);
  strcat(config.deviceName, tempStr);
  
  strcat(config.deviceName, "_S2:");
  itoa(currentReading.moisture2, tempStr, 10);
  strcat(config.deviceName, tempStr);
}

// ========================================================================
// PERIODIC DEVICE STATUS UPDATE
// ========================================================================
void updateDeviceStatus() {
  // Update device name every 30 seconds
  if (millis() - lastStatusUpdate >= 30000) {
    updateDeviceName();
    if (bluetoothConnected) {
      bluetooth.print(F("AT+GAPDEVNAME="));
      bluetooth.println(config.deviceName);
    }
    lastStatusUpdate = millis();
  }
}

// ========================================================================
// BLUETOOTH MESSAGE HANDLER
// ========================================================================
void handleBluetoothCommunication() {
  if (bluetooth.available()) {
    String message = bluetooth.readStringUntil('\n');
    message.trim();
    
    Serial.print(F("BT Received: "));
    Serial.println(message);
    
    // Check for connection status changes
    if (message.indexOf("CONNECTED") != -1) {
      bluetoothConnected = true;
      Serial.println(F("*** BLUETOOTH CONNECTED ***"));
      sendWelcomeMessage();
    }
    else if (message.indexOf("DISCONNECTED") != -1) {
      bluetoothConnected = false;
      Serial.println(F("*** BLUETOOTH DISCONNECTED ***"));
    }
    // Handle user commands
    else if (message.startsWith("CMD:")) {
      processUserCommand(message.substring(4));
    }
  }
}

// ========================================================================
// WELCOME MESSAGE FOR NEW CONNECTIONS
// ========================================================================
void sendWelcomeMessage() {
  bluetooth.println(F("POPUP:Soil Monitoring System Connected!"));
  delay(100);
  bluetooth.println(F("POPUP:=== CURRENT STATUS ==="));
  
  // Send sensor 1 status
  bluetooth.print(F("POPUP:Soil Sensor 1: "));
  bluetooth.print(currentReading.moisture1);
  bluetooth.print(F(" (Threshold: "));
  bluetooth.print(config.soilThreshold1);
  bluetooth.println(F(")"));
  
  // Send sensor 2 status  
  bluetooth.print(F("POPUP:Soil Sensor 2: "));
  bluetooth.print(currentReading.moisture2);
  bluetooth.print(F(" (Threshold: "));
  bluetooth.print(config.soilThreshold2);
  bluetooth.println(F(")"));
  
  delay(200);
  
  // Send available commands menu
  bluetooth.println(F("MENU:=== AVAILABLE COMMANDS ==="));
  bluetooth.println(F("MENU:1 - Current Readings"));
  bluetooth.println(F("MENU:2 - Set Sensor 1 Threshold"));
  bluetooth.println(F("MENU:3 - Set Sensor 2 Threshold"));
  bluetooth.println(F("MENU:4 - Toggle Alerts"));
  bluetooth.println(F("MENU:5 - Toggle Data Stream"));
  bluetooth.println(F("MENU:6 - System Statistics"));
  bluetooth.println(F("MENU:7 - Reset Counters"));
  bluetooth.println(F("MENU:0 - Show Menu"));
  bluetooth.println(F("MENU:Reply with command number"));
}

// ========================================================================
// USER COMMAND PROCESSOR
// ========================================================================
void processUserCommand(String command) {
  Serial.print(F("Processing command: "));
  Serial.println(command);
  
  if (command == "0") {
    // Show menu
    sendWelcomeMessage();
  }
  else if (command == "1") {
    // Current readings
    sendCurrentReadings();
  }
  else if (command == "2") {
    // Set threshold 1
    bluetooth.println(F("INPUT:Enter Sensor 1 threshold (0-1023):"));
    bluetooth.print(F("CURRENT:Current value: "));
    bluetooth.println(config.soilThreshold1);
  }
  else if (command == "3") {
    // Set threshold 2
    bluetooth.println(F("INPUT:Enter Sensor 2 threshold (0-1023):"));
    bluetooth.print(F("CURRENT:Current value: "));
    bluetooth.println(config.soilThreshold2);
  }
  else if (command == "4") {
    // Toggle alerts
    config.alertsEnabled = !config.alertsEnabled;
    bluetooth.print(F("POPUP:Alerts "));
    bluetooth.println(config.alertsEnabled ? F("ENABLED") : F("DISABLED"));
  }
  else if (command == "5") {
    // Toggle data streaming
    config.dataStreamEnabled = !config.dataStreamEnabled;
    bluetooth.print(F("POPUP:Data Stream "));
    bluetooth.println(config.dataStreamEnabled ? F("ENABLED") : F("DISABLED"));
  }
  else if (command == "6") {
    // System statistics
    sendSystemStatistics();
  }
  else if (command == "7") {
    // Reset counters
    totalReadings = 0;
    bluetooth.println(F("POPUP:Statistics reset!"));
  }
  else if (command.startsWith("SET1:")) {
    // Set threshold 1
    int newThreshold = command.substring(5).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.soilThreshold1 = newThreshold;
      bluetooth.print(F("POPUP:Sensor 1 threshold set to "));
      bluetooth.println(newThreshold);
      Serial.print(F("Threshold 1 updated to: "));
      Serial.println(newThreshold);
    } else {
      bluetooth.println(F("POPUP:ERROR: Value must be 0-1023"));
    }
  }
  else if (command.startsWith("SET2:")) {
    // Set threshold 2
    int newThreshold = command.substring(5).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.soilThreshold2 = newThreshold;
      bluetooth.print(F("POPUP:Sensor 2 threshold set to "));
      bluetooth.println(newThreshold);
      Serial.print(F("Threshold 2 updated to: "));
      Serial.println(newThreshold);
    } else {
      bluetooth.println(F("POPUP:ERROR: Value must be 0-1023"));
    }
  }
  else {
    bluetooth.println(F("POPUP:Unknown command. Send '0' for menu."));
  }
}

// ========================================================================
// SEND CURRENT SENSOR READINGS
// ========================================================================
void sendCurrentReadings() {
  bluetooth.println(F("DATA:=== CURRENT SENSOR READINGS ==="));
  
  // Sensor 1 reading
  bluetooth.print(F("DATA:Soil Sensor 1: "));
  bluetooth.print(currentReading.moisture1);
  if (currentReading.soil1Dry) {
    bluetooth.println(F(" (DRY - NEEDS ATTENTION)"));
  } else {
    bluetooth.println(F(" (MOIST - OK)"));
  }
  
  // Sensor 2 reading
  bluetooth.print(F("DATA:Soil Sensor 2: "));
  bluetooth.print(currentReading.moisture2);
  if (currentReading.soil2Dry) {
    bluetooth.println(F(" (DRY - NEEDS ATTENTION)"));
  } else {
    bluetooth.println(F(" (MOIST - OK)"));
  }
  
  // Thresholds and uptime
  bluetooth.print(F("DATA:Threshold 1: "));
  bluetooth.println(config.soilThreshold1);
  bluetooth.print(F("DATA:Threshold 2: "));
  bluetooth.println(config.soilThreshold2);
  bluetooth.print(F("DATA:System Uptime: "));
  bluetooth.print(millis() / 1000);
  bluetooth.println(F(" seconds"));
}

// ========================================================================
// SEND SYSTEM STATISTICS
// ========================================================================
void sendSystemStatistics() {
  bluetooth.println(F("DATA:=== SYSTEM STATISTICS ==="));
  bluetooth.print(F("DATA:Total Readings: "));
  bluetooth.println(totalReadings);
  bluetooth.print(F("DATA:Reading Interval: "));
  bluetooth.print(config.readingInterval / 1000);
  bluetooth.println(F(" seconds"));
  bluetooth.print(F("DATA:Alerts: "));
  bluetooth.println(config.alertsEnabled ? F("ENABLED") : F("DISABLED"));
  bluetooth.print(F("DATA:Data Stream: "));
  bluetooth.println(config.dataStreamEnabled ? F("ENABLED") : F("DISABLED"));
  bluetooth.print(F("DATA:System Uptime: "));
  bluetooth.print(millis() / 1000);
  bluetooth.println(F(" seconds"));
  bluetooth.print(F("DATA:Free Memory: "));
  bluetooth.print(getFreeMemory());
  bluetooth.println(F(" bytes"));
}

// ========================================================================
// TRANSMIT DATA STREAM
// ========================================================================
void transmitDataStream() {
  bluetooth.println(F("STREAM:DATA_UPDATE"));
  bluetooth.print(F("STREAM:"));
  bluetooth.print(currentReading.moisture1);
  bluetooth.print(F(","));
  bluetooth.print(currentReading.moisture2);
  bluetooth.print(F(","));
  bluetooth.print(millis());
  bluetooth.print(F(","));
  bluetooth.print(currentReading.soil1Dry ? 1 : 0);
  bluetooth.print(F(","));
  bluetooth.println(currentReading.soil2Dry ? 1 : 0);
}

// ========================================================================
// READ ALL SENSORS
// ========================================================================
void readSensors() {
  // Store previous reading for comparison
  previousReading = currentReading;
  
  // Read analog sensors (0-1023 range)
  currentReading.moisture1 = analogRead(SOIL_SENSOR_1_PIN);
  currentReading.moisture2 = analogRead(SOIL_SENSOR_2_PIN);
  currentReading.timestamp = millis();
  
  // Determine dry/wet status (higher values = drier soil for capacitive sensors)
  currentReading.soil1Dry = (currentReading.moisture1 > config.soilThreshold1);
  currentReading.soil2Dry = (currentReading.moisture2 > config.soilThreshold2);
  
  // Add small delay for ADC settling
  delay(10);
}

// ========================================================================
// CHECK FOR ALERT CONDITIONS
// ========================================================================
void checkForAlerts() {
  bool alertTriggered = false;
  
  // Check for newly dry conditions (state change from wet to dry)
  if (!previousReading.soil1Dry && currentReading.soil1Dry) {
    Serial.println(F("*** ALERT: Sensor 1 soil is now DRY! ***"));
    if (bluetoothConnected && config.alertsEnabled) {
      bluetooth.println(F("ALERT:Soil Sensor 1 needs attention!"));
      bluetooth.print(F("ALERT:Reading: "));
      bluetooth.print(currentReading.moisture1);
      bluetooth.print(F(" (Threshold: "));
      bluetooth.print(config.soilThreshold1);
      bluetooth.println(F(")"));
    }
    alertTriggered = true;
  }
  
  if (!previousReading.soil2Dry && currentReading.soil2Dry) {
    Serial.println(F("*** ALERT: Sensor 2 soil is now DRY! ***"));
    if (bluetoothConnected && config.alertsEnabled) {
      bluetooth.println(F("ALERT:Soil Sensor 2 needs attention!"));
      bluetooth.print(F("ALERT:Reading: "));
      bluetooth.print(currentReading.moisture2);
      bluetooth.print(F(" (Threshold: "));
      bluetooth.print(config.soilThreshold2);
      bluetooth.println(F(")"));
    }
    alertTriggered = true;
  }
  
  // Flash LED rapidly for new alerts
  if (alertTriggered) {
    for (int i = 0; i < 6; i++) {
      digitalWrite(STATUS_LED_PIN, HIGH);
      delay(100);
      digitalWrite(STATUS_LED_PIN, LOW);
      delay(100);
    }
  }
}

// ========================================================================
// LOG SENSOR DATA TO SERIAL MONITOR
// ========================================================================
void logSensorData() {
  Serial.print(F("Reading #"));
  Serial.print(totalReadings + 1);
  Serial.print(F(" | S1: "));
  Serial.print(currentReading.moisture1);
  Serial.print(currentReading.soil1Dry ? F(" (DRY)") : F(" (WET)"));
  Serial.print(F(" | S2: "));
  Serial.print(currentReading.moisture2);
  Serial.print(currentReading.soil2Dry ? F(" (DRY)") : F(" (WET)"));
  Serial.print(F(" | BT: "));
  Serial.println(bluetoothConnected ? F("CONNECTED") : F("DISCONNECTED"));
}

// ========================================================================
// UPDATE STATUS LED BASED ON SYSTEM STATE
// ========================================================================
void updateStatusLED() {
  if (bluetoothConnected) {
    // Connected states
    if (currentReading.soil1Dry || currentReading.soil2Dry) {
      // Fast blink: Connected + Alert condition
      digitalWrite(STATUS_LED_PIN, (millis() / 250) % 2);
    } else {
      // Solid on: Connected + All sensors OK
      digitalWrite(STATUS_LED_PIN, HIGH);
    }
  } else {
    // Disconnected states
    if (currentReading.soil1Dry || currentReading.soil2Dry) {
      // Medium blink: Disconnected + Alert condition
      digitalWrite(STATUS_LED_PIN, (millis() / 500) % 2);
    } else {
      // Slow blink: Disconnected + All sensors OK
      digitalWrite(STATUS_LED_PIN, (millis() / 1000) % 2);
    }
  }
}

// ========================================================================
// GET FREE MEMORY (USEFUL FOR DEBUGGING)
// ========================================================================
int getFreeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// ========================================================================
// END OF SOIL MOISTURE MONITORING SYSTEM
// ========================================================================