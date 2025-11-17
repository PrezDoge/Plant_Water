/*
 * H-Bridge Motor Test
 * 
 * Test your DRV8871 H-Bridge and DC motor connection before using the main sketch.
 * This will cycle the motor forward, stop, reverse, stop every few seconds.
 */

const int MOTOR_IN1_PIN = 7;  // DRV8871 IN1
const int MOTOR_IN2_PIN = 8;  // DRV8871 IN2
const int LED_PIN = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("DRV8871 H-Bridge Motor Test Starting...");
  Serial.println("Hardware: RedBoard + DRV8871 + DC Mini Motor");
  
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Ensure motor starts stopped
  stopMotor();
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("Motor will cycle: Forward 2s, Stop 1s, Reverse 2s, Stop 1s");
  Serial.println("Monitor your motor to ensure it's working correctly");
  Serial.println("Check buck converter output voltage for proper motor voltage");
}

void loop() {
  // Motor Forward
  Serial.println("Motor: FORWARD (Pumping)");
  forwardMotor();
  digitalWrite(LED_PIN, HIGH);
  delay(2000); // Run forward for 2 seconds
  
  // Motor Stop
  Serial.println("Motor: STOP");
  stopMotor();
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Stop for 1 second
  
  // Motor Reverse
  Serial.println("Motor: REVERSE (Priming)");
  reverseMotor();
  digitalWrite(LED_PIN, HIGH);
  delay(2000); // Run reverse for 2 seconds
  
  // Motor Stop
  Serial.println("Motor: STOP");
  stopMotor();
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Stop for 1 second
}

// Motor control functions
void forwardMotor() {
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}

void reverseMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, HIGH);
}

void stopMotor() {
  // Coast stop (both LOW) - motor can spin freely
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}

void brakeMotor() {
  // Brake stop (both HIGH) - motor brakes actively
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, HIGH);
}