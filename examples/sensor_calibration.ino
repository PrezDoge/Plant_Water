/*
 * Sensor Calibration Test - DFRobot SEN0114
 * 
 * Use this sketch to calibrate your DFRobot moisture sensor (SEN0114) and water level sensor.
 * This will help you determine the appropriate threshold values for your main sketch.
 */

const int SOIL_MOISTURE_PIN_3V = A0;  // 3.3V sensor on A0
const int SOIL_MOISTURE_PIN_5V = A1;  // 5V sensor on A1

void setup() {
  Serial.begin(9600);
  Serial.println("Dual Moisture Sensor Calibration Test");
  Serial.println("====================================");
  Serial.println("Hardware: Two moisture sensors at different voltages");
  Serial.println("A0: 3.3V sensor | A1: 5V sensor");
  Serial.println("");
  Serial.println("Instructions:");
  Serial.println("1. Place BOTH sensors in DRY soil - note HIGH values");
  Serial.println("2. Place BOTH sensors in WET soil - note LOW values");
  Serial.println("3. Update DRY_THRESHOLD_3V and DRY_THRESHOLD_5V in main sketch");
  Serial.println("4. 5V sensor typically reads higher values than 3.3V sensor");
  Serial.println("");
  Serial.println("Remember: Higher sensor values = Drier soil!");
  Serial.println("");
}

void loop() {
  int soilMoisture3V = analogRead(SOIL_MOISTURE_PIN_3V);
  int soilMoisture5V = analogRead(SOIL_MOISTURE_PIN_5V);
  
  Serial.print("3.3V Sensor (A0): ");
  Serial.print(soilMoisture3V);
  Serial.print(" | 5V Sensor (A1): ");
  Serial.print(soilMoisture5V);
  
  
  // Provide guidance for both sensors
  Serial.print(" | Comparison: ");
  if (soilMoisture3V > 500 || soilMoisture5V > 600) {
    Serial.println("DRY soil detected");
  } else if (soilMoisture3V > 300 || soilMoisture5V > 400) {
    Serial.println("MOIST soil detected");
  } else {
    Serial.println("WET soil detected");
  }
  
  // Show voltage difference effect
  float ratio = (soilMoisture5V > 0) ? (float)soilMoisture3V / soilMoisture5V : 0;
  Serial.print("Voltage ratio (3.3V/5V): ");
  Serial.println(ratio, 2);
  
  delay(2000); // Read every 2 seconds
}