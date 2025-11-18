/*
 * Plant Watering System - Basic Serial Version
 * CENG 3264 Project
 * 
 * This is the basic Arduino sketch for automated plant watering.
 * For web-based configuration interface, use plant_water_webconfig.ino
 * 
 * Hardware:
 * - SparkFun RedBoard (DEV-15025)
 * - Dual moisture sensors (3.3V + 5V)
 * - DRV8871 H-Bridge motor controller
 * - LM2596 Buck Converter
 * 
 * Features:
 * - Dual soil moisture monitoring
 * - H-Bridge motor control for water pump
 * - Serial monitor configuration
 * - Manual override controls
 */

// Pin definitions for SparkFun RedBoard (Arduino Uno compatible)
const int SOIL_MOISTURE_PIN_3V = A0; // Analog pin for 3.3V moisture sensor
const int SOIL_MOISTURE_PIN_5V = A1; // Analog pin for 5V moisture sensor
const int MOTOR_IN1_PIN = 7;         // DRV8871 H-Bridge input 1
const int MOTOR_IN2_PIN = 8;         // DRV8871 H-Bridge input 2
const int LED_PIN = 13;              // Built-in LED for status indication

// Sensor thresholds (configurable via serial commands)
int DRY_THRESHOLD_3V = 500;          // Threshold for 3.3V sensor (higher = drier)
int DRY_THRESHOLD_5V = 600;          // Threshold for 5V sensor (higher = drier)

// System variables
bool isWatering = false;
bool manualOverride = false;
int soilMoisture3V = 0;
int soilMoisture5V = 0;

// Timing variables
unsigned long lastWateringTime = 0;
unsigned long wateringDuration = 5000;     // 5 seconds of watering
unsigned long measurementInterval = 30000; // Check sensors every 30 seconds
unsigned long lastMeasurement = 0;

void setup() {
  Serial.begin(9600);
  
  Serial.println("=================================");
  Serial.println("  Plant Watering System v3.0");
  Serial.println("  CENG 3264 Project");
  Serial.println("  Basic Serial Mode");
  Serial.println("=================================");
  Serial.println();
  Serial.println("NOTE: For web interface, use plant_water_webconfig.ino");
  Serial.println("Hardware: RedBoard + DRV8871 H-Bridge + Dual Moisture Sensors");
  Serial.println();
  
  // Initialize pins
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Ensure motor is stopped at startup
  stopMotor();
  digitalWrite(LED_PIN, LOW);
  
  // Display current thresholds
  Serial.print("Current thresholds - 3.3V sensor: ");
  Serial.print(DRY_THRESHOLD_3V);
  Serial.print(", 5V sensor: ");
  Serial.println(DRY_THRESHOLD_5V);
  Serial.println();
  Serial.println("Serial Commands:");
  Serial.println("  SET3V:XXX  - Set 3.3V threshold");
  Serial.println("  SET5V:XXX  - Set 5V threshold");
  Serial.println("  WATER      - Manual watering");
  Serial.println("  STOP       - Stop watering");
  Serial.println("  STATUS     - Show current status");
  Serial.println();
  
  Serial.println("System initialized successfully!");
  
  // Initial sensor reading
  readSensors();
}

void loop() {
  // Handle serial commands
  checkSerialCommands();
  
  // Read sensors at regular intervals
  if (millis() - lastMeasurement >= measurementInterval) {
    readSensors();
    checkWateringNeeded();
    displayStatus();
    lastMeasurement = millis();
  }
  
  // Update LED status
  updateStatusLED();
  
  // Stop watering after duration
  if (isWatering && (millis() - lastWateringTime >= wateringDuration)) {
    stopWatering();
  }
  
  delay(100); // Small delay for stability
}

void readSensors() {
  soilMoisture3V = analogRead(SOIL_MOISTURE_PIN_3V);
  soilMoisture5V = analogRead(SOIL_MOISTURE_PIN_5V);
  
  Serial.print("Sensor readings - 3.3V: ");
  Serial.print(soilMoisture3V);
  Serial.print(" (");
  Serial.print(soilMoisture3V > DRY_THRESHOLD_3V ? "DRY" : "WET");
  Serial.print("), 5V: ");
  Serial.print(soilMoisture5V);
  Serial.print(" (");
  Serial.print(soilMoisture5V > DRY_THRESHOLD_5V ? "DRY" : "WET");
  Serial.println(")");
}

void checkWateringNeeded() {
  if (manualOverride || isWatering) return;
  
  // Check if either sensor indicates dry soil
  bool need3VWatering = soilMoisture3V > DRY_THRESHOLD_3V;
  bool need5VWatering = soilMoisture5V > DRY_THRESHOLD_5V;
  
  if (need3VWatering || need5VWatering) {
    Serial.println("*** AUTOMATIC WATERING TRIGGERED ***");
    startWatering(false);
  }
}

void startWatering(bool manual) {
  isWatering = true;
  manualOverride = manual;
  lastWateringTime = millis();
  
  // Start motor (forward direction)
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  
  Serial.print("Watering started - ");
  Serial.print(manual ? "MANUAL" : "AUTOMATIC");
  Serial.print(" (Duration: ");
  Serial.print(wateringDuration / 1000);
  Serial.println(" seconds)");
}

void stopWatering() {
  isWatering = false;
  manualOverride = false;
  
  // Stop motor
  stopMotor();
  
  Serial.println("Watering stopped");
}

void stopMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}



void updateStatusLED() {
  if (isWatering) {
    // Blink rapidly when watering
    digitalWrite(LED_PIN, (millis() / 200) % 2);
  } else {
    // Slow blink when idle
    digitalWrite(LED_PIN, (millis() / 1000) % 2);
  }
}

void displayStatus() {
  Serial.println("--- System Status ---");
  Serial.print("Motor: ");
  Serial.println(isWatering ? "RUNNING" : "STOPPED");
  Serial.print("Mode: ");
  Serial.println(manualOverride ? "MANUAL" : "AUTOMATIC");
  Serial.print("3.3V Sensor: ");
  Serial.print(soilMoisture3V);
  Serial.print(" / ");
  Serial.print(DRY_THRESHOLD_3V);
  Serial.print(" (");
  Serial.print(soilMoisture3V > DRY_THRESHOLD_3V ? "DRY" : "WET");
  Serial.println(")");
  Serial.print("5V Sensor: ");
  Serial.print(soilMoisture5V);
  Serial.print(" / ");
  Serial.print(DRY_THRESHOLD_5V);
  Serial.print(" (");
  Serial.print(soilMoisture5V > DRY_THRESHOLD_5V ? "DRY" : "WET");
  Serial.println(")");
  Serial.println("--------------------");
}

void checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    if (command.startsWith("SET3V:")) {
      int newThreshold = command.substring(6).toInt();
      if (newThreshold >= 0 && newThreshold <= 1023) {
        DRY_THRESHOLD_3V = newThreshold;
        Serial.print("3.3V threshold set to: ");
        Serial.println(DRY_THRESHOLD_3V);
      } else {
        Serial.println("Error: Threshold must be 0-1023");
      }
    }
    else if (command.startsWith("SET5V:")) {
      int newThreshold = command.substring(6).toInt();
      if (newThreshold >= 0 && newThreshold <= 1023) {
        DRY_THRESHOLD_5V = newThreshold;
        Serial.print("5V threshold set to: ");
        Serial.println(DRY_THRESHOLD_5V);
      } else {
        Serial.println("Error: Threshold must be 0-1023");
      }
    }
    else if (command == "WATER") {
      if (!isWatering) {
        startWatering(true);
      } else {
        Serial.println("Already watering");
      }
    }
    else if (command == "STOP") {
      if (isWatering) {
        stopWatering();
      } else {
        Serial.println("Not currently watering");
      }
    }
    else if (command == "STATUS") {
      readSensors();
      displayStatus();
    }
    else {
      Serial.println("Unknown command. Available: SET3V:XXX, SET5V:XXX, WATER, STOP, STATUS");
    }
  }
}