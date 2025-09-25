/*
 * Component Test Sketch for Multi-Mode Adaptive Lighting Control System
 * 
 * This sketch is used to test individual components before running the main program.
 * Uncomment the test functions you want to run in the loop() function.
 * 
 * Use this for:
 * - Verifying hardware connections
 * - Testing component functionality
 * - Debugging sensor readings
 * - Calibrating sensor ranges
 */

// Pin Definitions (same as main sketch)
const int MAIN_LED_PIN = 9;
const int POTENTIOMETER_PIN = A0;
const int BUTTON_PIN = 2;
const int SOUND_SENSOR_PIN = A1;
const int PHOTORESISTOR_PIN = A2;
const int GREEN_LED_PIN = 3;
const int RED_LED_PIN = 4;
const int YELLOW_LED_PIN = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("=== Component Test Suite ===");
  Serial.println("Uncomment test functions in loop() to run specific tests");
  
  // Configure pin modes
  pinMode(MAIN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  
  Serial.println("Setup complete. Starting tests...");
}

void loop() {
  // Uncomment the tests you want to run:
  
  // testAllLEDs();         // Test all LEDs sequentially
  // testButton();          // Test button presses
  // testPotentiometer();   // Test potentiometer readings
  // testSoundSensor();     // Test sound sensor
  // testPhotoresistor();   // Test light sensor
  // testMainLEDFade();     // Test main LED PWM fade
  
  // Default: Run all sensors continuously
  testAllSensorsContinuous();
  
  delay(100);
}

// Test all LEDs in sequence
void testAllLEDs() {
  Serial.println("Testing LEDs...");
  
  // Test each LED
  digitalWrite(GREEN_LED_PIN, HIGH);
  Serial.println("Green LED ON");
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  digitalWrite(RED_LED_PIN, HIGH);
  Serial.println("Red LED ON");
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  
  digitalWrite(YELLOW_LED_PIN, HIGH);
  Serial.println("Yellow LED ON");
  delay(1000);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  // Test main LED at different brightness levels
  Serial.println("Testing Main LED brightness...");
  for (int i = 0; i <= 255; i += 51) {
    analogWrite(MAIN_LED_PIN, i);
    Serial.print("Main LED brightness: ");
    Serial.println(i);
    delay(500);
  }
  analogWrite(MAIN_LED_PIN, 0);
  
  Serial.println("LED test complete\n");
}

// Test button functionality
void testButton() {
  static bool lastButtonState = LOW;
  static unsigned long pressTime = 0;
  
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == HIGH) {
      pressTime = millis();
      Serial.println("Button PRESSED");
    } else {
      unsigned long duration = millis() - pressTime;
      Serial.print("Button RELEASED after ");
      Serial.print(duration);
      Serial.println(" ms");
      
      if (duration > 2000) {
        Serial.println("-> LONG PRESS detected");
      } else if (duration > 50) {
        Serial.println("-> SHORT PRESS detected");
      }
    }
    lastButtonState = currentButtonState;
  }
}

// Test potentiometer readings
void testPotentiometer() {
  static unsigned long lastPrint = 0;
  
  if (millis() - lastPrint > 200) {
    int potValue = analogRead(POTENTIOMETER_PIN);
    int percentage = map(potValue, 0, 1023, 0, 100);
    
    Serial.print("Potentiometer - Raw: ");
    Serial.print(potValue);
    Serial.print(" (");
    Serial.print(percentage);
    Serial.println("%)");
    
    // Control main LED with potentiometer
    int brightness = map(potValue, 0, 1023, 0, 255);
    analogWrite(MAIN_LED_PIN, brightness);
    
    lastPrint = millis();
  }
}

// Test sound sensor
void testSoundSensor() {
  static unsigned long lastPrint = 0;
  static int maxSound = 0;
  static int minSound = 1023;
  
  if (millis() - lastPrint > 100) {
    int soundLevel = analogRead(SOUND_SENSOR_PIN);
    
    // Track min/max for calibration
    if (soundLevel > maxSound) maxSound = soundLevel;
    if (soundLevel < minSound) minSound = soundLevel;
    
    Serial.print("Sound Level: ");
    Serial.print(soundLevel);
    Serial.print(" | Range: ");
    Serial.print(minSound);
    Serial.print("-");
    Serial.print(maxSound);
    
    // Visual indication of loud sounds
    if (soundLevel > 600) {
      Serial.print(" <- LOUD!");
      digitalWrite(RED_LED_PIN, HIGH);
      analogWrite(MAIN_LED_PIN, 255);
    } else {
      digitalWrite(RED_LED_PIN, LOW);
      analogWrite(MAIN_LED_PIN, 0);
    }
    
    Serial.println();
    lastPrint = millis();
  }
}

// Test photoresistor (LDR)
void testPhotoresistor() {
  static unsigned long lastPrint = 0;
  static int maxLight = 0;
  static int minLight = 1023;
  
  if (millis() - lastPrint > 200) {
    int lightLevel = analogRead(PHOTORESISTOR_PIN);
    
    // Track min/max for calibration
    if (lightLevel > maxLight) maxLight = lightLevel;
    if (lightLevel < minLight) minLight = lightLevel;
    
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.print(" | Range: ");
    Serial.print(minLight);
    Serial.print("-");
    Serial.print(maxLight);
    
    // Inverse relationship: dark = bright LED
    int brightness = map(lightLevel, minLight, maxLight, 255, 0);
    brightness = constrain(brightness, 0, 255);
    
    Serial.print(" | LED Brightness: ");
    Serial.print(brightness);
    
    if (lightLevel < 300) {
      Serial.print(" <- DARK");
      digitalWrite(YELLOW_LED_PIN, HIGH);
    } else if (lightLevel > 700) {
      Serial.print(" <- BRIGHT");
      digitalWrite(YELLOW_LED_PIN, LOW);
    }
    
    analogWrite(MAIN_LED_PIN, brightness);
    Serial.println();
    lastPrint = millis();
  }
}

// Test main LED fade effect
void testMainLEDFade() {
  static int brightness = 0;
  static int fadeDirection = 1;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 20) {
    brightness += fadeDirection * 5;
    
    if (brightness <= 0) {
      brightness = 0;
      fadeDirection = 1;
    } else if (brightness >= 255) {
      brightness = 255;
      fadeDirection = -1;
    }
    
    analogWrite(MAIN_LED_PIN, brightness);
    
    if (brightness == 0 || brightness == 255) {
      Serial.print("Fade brightness: ");
      Serial.println(brightness);
    }
    
    lastUpdate = millis();
  }
}

// Test all sensors continuously (default test)
void testAllSensorsContinuous() {
  static unsigned long lastPrint = 0;
  
  if (millis() - lastPrint > 500) {
    Serial.println("=== Continuous Sensor Readings ===");
    
    // Read all sensors
    int potValue = analogRead(POTENTIOMETER_PIN);
    int soundValue = analogRead(SOUND_SENSOR_PIN);
    int lightValue = analogRead(PHOTORESISTOR_PIN);
    bool buttonState = digitalRead(BUTTON_PIN);
    
    // Display readings
    Serial.print("Pot: ");
    Serial.print(potValue);
    Serial.print(" | Sound: ");
    Serial.print(soundValue);
    Serial.print(" | Light: ");
    Serial.print(lightValue);
    Serial.print(" | Button: ");
    Serial.println(buttonState ? "PRESSED" : "RELEASED");
    
    // Visual feedback
    digitalWrite(GREEN_LED_PIN, potValue > 512);
    digitalWrite(RED_LED_PIN, soundValue > 600);
    digitalWrite(YELLOW_LED_PIN, lightValue < 400);
    
    // Main LED brightness based on potentiometer
    int brightness = map(potValue, 0, 1023, 0, 255);
    analogWrite(MAIN_LED_PIN, brightness);
    
    lastPrint = millis();
  }
}

// Calibration helper functions
void printCalibrationData() {
  Serial.println("\n=== Calibration Data Collection ===");
  Serial.println("Move potentiometer, make sounds, change lighting...");
  
  int potMin = 1023, potMax = 0;
  int soundMin = 1023, soundMax = 0;
  int lightMin = 1023, lightMax = 0;
  
  for (int i = 0; i < 1000; i++) {
    int pot = analogRead(POTENTIOMETER_PIN);
    int sound = analogRead(SOUND_SENSOR_PIN);
    int light = analogRead(PHOTORESISTOR_PIN);
    
    potMin = min(potMin, pot);
    potMax = max(potMax, pot);
    soundMin = min(soundMin, sound);
    soundMax = max(soundMax, sound);
    lightMin = min(lightMin, light);
    lightMax = max(lightMax, light);
    
    if (i % 100 == 0) {
      Serial.print(".");
    }
    delay(10);
  }
  
  Serial.println("\nCalibration Results:");
  Serial.print("Potentiometer range: ");
  Serial.print(potMin);
  Serial.print(" - ");
  Serial.println(potMax);
  
  Serial.print("Sound range: ");
  Serial.print(soundMin);
  Serial.print(" - ");
  Serial.println(soundMax);
  
  Serial.print("Light range: ");
  Serial.print(lightMin);
  Serial.print(" - ");
  Serial.println(lightMax);
}