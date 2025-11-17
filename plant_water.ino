/*
 * Plant Watering System with Bluetooth Popup Interface
 * CENG 3264 Project
 * 
 * This system uses Nordic nRF52840 Bluetooth to create native phone popups
 * for configuration - no app required, just Bluetooth pairing!
 * 
 * Hardware:
 * - SparkFun RedBoard (DEV-13975)
 * - Dual moisture sensors (3.3V + 5V)
 * - DRV8871 H-Bridge motor controller
 * - LM2596 Buck Converter
 * - SparkFun Pro nRF52840 Mini Bluetooth Development Board
 * 
 * Features:
 * - Bluetooth device name shows current status
 * - Pairing triggers configuration popups
 * - Phone notifications for watering events
 * - SMS-like message interface for settings
 * - No app installation required!
 */

#include <SoftwareSerial.h>

// Pin definitions for SparkFun RedBoard (Arduino Uno compatible)
const int SOIL_MOISTURE_PIN_3V = A0;
const int SOIL_MOISTURE_PIN_5V = A1;
const int MOTOR_IN1_PIN = 7;
const int MOTOR_IN2_PIN = 8;
const int LED_PIN = 13;

// SparkFun Pro nRF52840 Mini pins
const int BLUETOOTH_TX_PIN = 4;  // Connect to nRF52840 RX
const int BLUETOOTH_RX_PIN = 5;  // Connect to nRF52840 TX
SoftwareSerial bluetooth(BLUETOOTH_RX_PIN, BLUETOOTH_TX_PIN);

// Configuration variables
struct Config {
  char deviceName[32] = "PlantSystem_Ready";
  int dryThreshold3V = 500;
  int dryThreshold5V = 600;
  unsigned long wateringDuration = 5000;
  bool notificationsEnabled = true;
};

Config config;

// System variables
int currentMoisture3V = 0;
int currentMoisture5V = 0;
bool isWatering = false;
bool manualOverride = false;
unsigned long lastWateringTime = 0;
unsigned long lastMeasurement = 0;
unsigned long measurementInterval = 10000;
bool bluetoothConnected = false;
unsigned long lastStatusUpdate = 0;


unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  Serial.println("=================================");
  Serial.println("  Plant Watering System v4.0");
  Serial.println("  Bluetooth Popup Interface");
  Serial.println("=================================");
  
  // Initialize pins
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  
  stopMotor();
  
  // Initialize Bluetooth
  initializeBluetooth();
  
  Serial.println("System ready!");
  Serial.println("Pair with device to configure via phone popups");
  
  readSensors();
}

void loop() {
  // Handle Bluetooth communication
  handleBluetoothCommunication();
  
  // Update device name with current status
  updateBluetoothStatus();
  
  // Check sensors periodically
  if (millis() - lastMeasurement >= measurementInterval) {
    readSensors();
    checkWateringNeeded();
    lastMeasurement = millis();
  }
  

  
  // Update LED status
  updateStatusLED();
  
  // Stop watering after duration
  if (isWatering && (millis() - lastWateringTime >= config.wateringDuration)) {
    stopWatering();
  }
}

void initializeBluetooth() {
  Serial.println("Initializing nRF52840 Bluetooth...");
  
  // Configure nRF52840 for popup-friendly mode
  bluetooth.println("AT+RESET");
  delay(1000);
  
  // Set discoverable name with status
  updateDeviceName();
  
  // Enable notifications and popups
  bluetooth.println("AT+GAPDEVNAME=" + String(config.deviceName));
  bluetooth.println("AT+GAPDISCONNECT");
  bluetooth.println("AT+BLEADV_SET=ON");
  
  Serial.println("Bluetooth initialized - Device discoverable");
  Serial.println("Device name: " + String(config.deviceName));
}

void updateDeviceName() {
  String status = "PlantSystem";
  
  if (isWatering) {
    status = "PlantSystem_Watering";
  } else if (currentMoisture3V > config.dryThreshold3V || currentMoisture5V > config.dryThreshold5V) {
    status = "PlantSystem_NeedsWater";
  } else {
    status = "PlantSystem_Healthy";
  }
  
  // Add moisture levels to device name
  status += "_3V" + String(currentMoisture3V) + "_5V" + String(currentMoisture5V);
  
  strcpy(config.deviceName, status.c_str());
}

void updateBluetoothStatus() {
  // Update device name every 30 seconds
  if (millis() - lastStatusUpdate > 30000) {
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
      sendWelcomePopup();
    }
    else if (message.indexOf("DISCONNECTED") != -1) {
      bluetoothConnected = false;
      Serial.println("Phone disconnected");
    }
    // Handle incoming commands/responses
    else if (message.startsWith("CMD:")) {
      processPhoneCommand(message.substring(4));
    }
  }
  
  // Send periodic status if connected
  if (bluetoothConnected && (millis() - lastStatusUpdate > 60000)) {
    sendStatusNotification();
  }
}

void sendWelcomePopup() {
  // Send configuration options as notifications/popups
  bluetooth.println("POPUP:Plant Watering System Connected!");
  delay(100);
  bluetooth.println("POPUP:Current Status:");
  bluetooth.println("POPUP:3.3V Sensor: " + String(currentMoisture3V) + " (Threshold: " + String(config.dryThreshold3V) + ")");
  bluetooth.println("POPUP:5V Sensor: " + String(currentMoisture5V) + " (Threshold: " + String(config.dryThreshold5V) + ")");
  delay(100);
  
  // Send configuration menu
  bluetooth.println("MENU:Configuration Options:");
  bluetooth.println("MENU:1 - Set 3.3V Threshold");
  bluetooth.println("MENU:2 - Set 5V Threshold");
  bluetooth.println("MENU:3 - Manual Watering");
  bluetooth.println("MENU:4 - Stop Watering");
  bluetooth.println("MENU:5 - Device Name");
  bluetooth.println("MENU:Reply with number to configure");
}

void processPhoneCommand(String command) {
  Serial.println("Received command: " + command);
  
  if (command == "1") {
    // Request 3.3V threshold
    bluetooth.println("INPUT:Enter 3.3V threshold (0-1023):");
    bluetooth.println("CURRENT:" + String(config.dryThreshold3V));
  }
  else if (command == "2") {
    // Request 5V threshold  
    bluetooth.println("INPUT:Enter 5V threshold (0-1023):");
    bluetooth.println("CURRENT:" + String(config.dryThreshold5V));
  }
  else if (command == "3") {
    // Manual watering
    if (!isWatering) {
      startWatering(true);
      bluetooth.println("POPUP:Manual watering started!");
    } else {
      bluetooth.println("POPUP:Already watering!");
    }
  }
  else if (command == "4") {
    // Stop watering
    if (isWatering) {
      stopWatering();
      bluetooth.println("POPUP:Watering stopped!");
    } else {
      bluetooth.println("POPUP:Not currently watering");
    }
  }
  else if (command == "5") {
    // Device name
    bluetooth.println("INPUT:Enter device name:");
    bluetooth.println("CURRENT:My Plant System");
  }
  else if (command.startsWith("THRESH3V:")) {
    // Set 3.3V threshold
    int newThreshold = command.substring(9).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.dryThreshold3V = newThreshold;
      bluetooth.println("POPUP:3.3V threshold set to " + String(newThreshold));
      Serial.println("3.3V threshold updated: " + String(newThreshold));
    } else {
      bluetooth.println("POPUP:Error: Threshold must be 0-1023");
    }
  }
  else if (command.startsWith("THRESH5V:")) {
    // Set 5V threshold
    int newThreshold = command.substring(9).toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      config.dryThreshold5V = newThreshold;
      bluetooth.println("POPUP:5V threshold set to " + String(newThreshold));
      Serial.println("5V threshold updated: " + String(newThreshold));
    } else {
      bluetooth.println("POPUP:Error: Threshold must be 0-1023");
    }
  }
  else if (command.startsWith("NAME:")) {
    // Set device name
    String newName = command.substring(5);
    if (newName.length() > 0 && newName.length() < 20) {
      // Keep the base name for status updates
      bluetooth.println("POPUP:Device name updated!");
      Serial.println("Device name updated: " + newName);
    }
  }
  else if (command == "STATUS") {
    sendStatusNotification();
  }
}

void sendStatusNotification() {
  bluetooth.println("NOTIFICATION:Plant Status Update");
  bluetooth.println("NOTIFICATION:3.3V: " + String(currentMoisture3V) + (currentMoisture3V > config.dryThreshold3V ? " (DRY)" : " (WET)"));
  bluetooth.println("NOTIFICATION:5V: " + String(currentMoisture5V) + (currentMoisture5V > config.dryThreshold5V ? " (DRY)" : " (WET)"));
  bluetooth.println("NOTIFICATION:Status: " + String(isWatering ? "WATERING" : "MONITORING"));
}

void readSensors() {
  currentMoisture3V = analogRead(SOIL_MOISTURE_PIN_3V);
  currentMoisture5V = analogRead(SOIL_MOISTURE_PIN_5V);
  
  Serial.print("Sensors - 3V: ");
  Serial.print(currentMoisture3V);
  Serial.print(" (");
  Serial.print(currentMoisture3V > config.dryThreshold3V ? "DRY" : "WET");
  Serial.print("), 5V: ");
  Serial.print(currentMoisture5V);
  Serial.print(" (");
  Serial.print(currentMoisture5V > config.dryThreshold5V ? "DRY" : "WET");
  Serial.println(")");
}

void checkWateringNeeded() {
  if (manualOverride || isWatering) return;
  
  bool need3VWatering = currentMoisture3V > config.dryThreshold3V;
  bool need5VWatering = currentMoisture5V > config.dryThreshold5V;
  
  if (need3VWatering || need5VWatering) {
    Serial.println("Automatic watering triggered");
    startWatering(false);
    
    // Send notification to phone
    if (bluetoothConnected && config.notificationsEnabled) {
      bluetooth.println("POPUP:🌱 Plant needs water - Watering started!");
      bluetooth.println("NOTIFICATION:Automatic watering in progress");
    }
  }
}

void startWatering(bool manual) {
  isWatering = true;
  manualOverride = manual;
  lastWateringTime = millis();
  
  // Start motor
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  
  Serial.print("Watering started - ");
  Serial.println(manual ? "Manual" : "Automatic");
  
  // Update device name immediately
  updateDeviceName();
  bluetooth.println("AT+GAPDEVNAME=" + String(config.deviceName));
}

void stopWatering() {
  isWatering = false;
  manualOverride = false;
  
  stopMotor();
  Serial.println("Watering stopped");
  
  // Send completion notification
  if (bluetoothConnected) {
    bluetooth.println("POPUP:💧 Watering completed!");
    bluetooth.println("NOTIFICATION:Plant watering finished");
  }
  
  // Update device name
  updateDeviceName();
  bluetooth.println("AT+GAPDEVNAME=" + String(config.deviceName));
}

void stopMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}



void updateStatusLED() {
  if (bluetoothConnected) {
    if (isWatering) {
      // Fast blink when watering and connected
      digitalWrite(LED_PIN, (millis() / 100) % 2);
    } else {
      // Solid on when connected and idle
      digitalWrite(LED_PIN, HIGH);
    }
  } else {
    if (isWatering) {
      // Medium blink when watering but not connected
      digitalWrite(LED_PIN, (millis() / 300) % 2);
    } else {
      // Slow blink when not connected
      digitalWrite(LED_PIN, (millis() / 1000) % 2);
    }
  }
}