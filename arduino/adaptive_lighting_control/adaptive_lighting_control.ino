/*
 * Multi-Mode Adaptive Lighting Control System
 * 
 * This Arduino sketch implements a sophisticated lighting control system with three operating modes:
 * 1. Manual Mode (Default) - Potentiometer controlled brightness with green status LED
 * 2. Sound Control Mode - Sound-triggered brightness control with red status LED  
 * 3. Automatic Light Control Mode - Photoresistor-based adaptive brightness with yellow status LED
 * 
 * Hardware Requirements:
 * - Arduino Uno/Nano
 * - Main LED (PWM controllable)
 * - Potentiometer (10kΩ)
 * - Push button with pull-down resistor
 * - Sound sensor module (microphone)
 * - Photoresistor (LDR) with voltage divider
 * - Status LEDs: Red, Green, Yellow
 * - Appropriate resistors for LEDs
 * 
 * Author: Arduino Lighting Control System
 * Version: 1.0
 */

// Pin Definitions
const int MAIN_LED_PIN = 9;        // PWM pin for main controllable LED
const int POTENTIOMETER_PIN = A0;   // Analog pin for potentiometer
const int BUTTON_PIN = 2;           // Digital pin for mode button (with pull-down resistor)
const int SOUND_SENSOR_PIN = A1;    // Analog pin for sound sensor
const int PHOTORESISTOR_PIN = A2;   // Analog pin for photoresistor (LDR)
const int GREEN_LED_PIN = 3;        // Status LED for Manual Mode
const int RED_LED_PIN = 4;          // Status LED for Sound Control Mode
const int YELLOW_LED_PIN = 5;       // Status LED for Automatic Mode

// Operating Modes
enum LightingMode {
  MANUAL_MODE = 0,
  SOUND_CONTROL_MODE = 1,
  AUTOMATIC_MODE = 2
};

// Global Variables
LightingMode currentMode = MANUAL_MODE;
int mainLEDBrightness = 0;

// Button handling variables
volatile bool buttonPressed = false;
unsigned long buttonPressTime = 0;
unsigned long lastButtonTime = 0;
int clickCount = 0;
bool longPressDetected = false;

// Timing constants
const unsigned long DEBOUNCE_DELAY = 50;     // 50ms debounce
const unsigned long LONG_PRESS_TIME = 2000;  // 2 seconds for long press
const unsigned long DOUBLE_CLICK_TIME = 300; // 300ms window for double click

// Sound control variables
int soundThreshold = 512;           // Adjustable sound threshold (0-1023)
unsigned long soundTriggerTime = 0;
const unsigned long SOUND_ACTIVE_TIME = 3000; // 3 seconds of full brightness after sound trigger

// Automatic mode variables
int lightCalibrationMin = 100;      // Minimum light reading (dark)
int lightCalibrationMax = 900;      // Maximum light reading (bright)

// Smoothing variables for analog readings
const int SMOOTHING_SAMPLES = 10;
int potReadings[SMOOTHING_SAMPLES];
int soundReadings[SMOOTHING_SAMPLES];
int lightReadings[SMOOTHING_SAMPLES];
int readIndex = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Multi-Mode Adaptive Lighting Control System Starting...");
  
  // Configure pin modes
  pinMode(MAIN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  
  // Initialize smoothing arrays
  for (int i = 0; i < SMOOTHING_SAMPLES; i++) {
    potReadings[i] = 0;
    soundReadings[i] = 0;
    lightReadings[i] = 0;
  }
  
  // Set up button interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
  
  // Initialize in Manual Mode
  setMode(MANUAL_MODE);
  
  // Perform initial sensor calibration
  calibrateSensors();
  
  Serial.println("System initialized successfully!");
  Serial.println("Modes: Single click = Sound Control, Long press = Automatic, Double click = Manual");
}

void loop() {
  // Handle button press detection
  handleButtonPress();
  
  // Execute current mode logic
  switch (currentMode) {
    case MANUAL_MODE:
      executeManualMode();
      break;
    case SOUND_CONTROL_MODE:
      executeSoundControlMode();
      break;
    case AUTOMATIC_MODE:
      executeAutomaticMode();
      break;
  }
  
  // Update main LED brightness
  analogWrite(MAIN_LED_PIN, mainLEDBrightness);
  
  // Small delay for system stability
  delay(10);
}

// Interrupt Service Routine for button press
void buttonISR() {
  buttonPressed = true;
  buttonPressTime = millis();
}

// Handle button press logic with debouncing and click detection
void handleButtonPress() {
  if (buttonPressed) {
    unsigned long currentTime = millis();
    
    // Debounce check
    if (currentTime - lastButtonTime > DEBOUNCE_DELAY) {
      // Wait for button release or long press detection
      while (digitalRead(BUTTON_PIN) == HIGH) {
        if (millis() - buttonPressTime > LONG_PRESS_TIME && !longPressDetected) {
          longPressDetected = true;
          setMode(AUTOMATIC_MODE);
          Serial.println("Long press detected - Switching to Automatic Mode");
          break;
        }
        delay(10);
      }
      
      // If not a long press, handle as click
      if (!longPressDetected) {
        clickCount++;
        
        // Wait for potential second click
        unsigned long waitStart = millis();
        while (millis() - waitStart < DOUBLE_CLICK_TIME) {
          if (digitalRead(BUTTON_PIN) == HIGH) {
            // Second click detected
            delay(50); // Debounce
            while (digitalRead(BUTTON_PIN) == HIGH) delay(10); // Wait for release
            clickCount++;
            break;
          }
          delay(10);
        }
        
        // Process clicks
        if (clickCount == 1) {
          setMode(SOUND_CONTROL_MODE);
          Serial.println("Single click detected - Switching to Sound Control Mode");
        } else if (clickCount >= 2) {
          setMode(MANUAL_MODE);
          Serial.println("Double click detected - Switching to Manual Mode");
        }
      }
      
      lastButtonTime = currentTime;
    }
    
    // Reset button variables
    buttonPressed = false;
    clickCount = 0;
    longPressDetected = false;
  }
}

// Set operating mode and update status LEDs
void setMode(LightingMode newMode) {
  currentMode = newMode;
  
  // Turn off all status LEDs
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  // Turn on appropriate status LED
  switch (currentMode) {
    case MANUAL_MODE:
      digitalWrite(GREEN_LED_PIN, HIGH);
      Serial.println("Mode: MANUAL (Green LED)");
      break;
    case SOUND_CONTROL_MODE:
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("Mode: SOUND CONTROL (Red LED)");
      break;
    case AUTOMATIC_MODE:
      digitalWrite(YELLOW_LED_PIN, HIGH);
      Serial.println("Mode: AUTOMATIC (Yellow LED)");
      break;
  }
}

// Execute Manual Mode logic
void executeManualMode() {
  // Read potentiometer with smoothing
  int potValue = smoothAnalogRead(POTENTIOMETER_PIN, potReadings);
  
  // Map potentiometer value to LED brightness (0-255)
  mainLEDBrightness = map(potValue, 0, 1023, 0, 255);
  
  // Optional: Print debug information periodically
  static unsigned long lastDebugTime = 0;
  if (millis() - lastDebugTime > 1000) {
    Serial.print("Manual Mode - Pot: ");
    Serial.print(potValue);
    Serial.print(", Brightness: ");
    Serial.println(mainLEDBrightness);
    lastDebugTime = millis();
  }
}

// Execute Sound Control Mode logic
void executeSoundControlMode() {
  // Read sound sensor with smoothing
  int soundLevel = smoothAnalogRead(SOUND_SENSOR_PIN, soundReadings);
  
  // Check if sound exceeds threshold
  if (soundLevel > soundThreshold) {
    soundTriggerTime = millis();
    mainLEDBrightness = 255; // Full brightness
    Serial.print("Sound triggered! Level: ");
    Serial.println(soundLevel);
  }
  
  // Gradually decrease brightness after sound trigger
  if (millis() - soundTriggerTime > SOUND_ACTIVE_TIME) {
    mainLEDBrightness = max(0, mainLEDBrightness - 2); // Gradual fade
  }
  
  // Optional: Print debug information periodically
  static unsigned long lastDebugTime = 0;
  if (millis() - lastDebugTime > 1000) {
    Serial.print("Sound Mode - Level: ");
    Serial.print(soundLevel);
    Serial.print(", Threshold: ");
    Serial.print(soundThreshold);
    Serial.print(", Brightness: ");
    Serial.println(mainLEDBrightness);
    lastDebugTime = millis();
  }
}

// Execute Automatic Mode logic
void executeAutomaticMode() {
  // Read photoresistor with smoothing
  int lightLevel = smoothAnalogRead(PHOTORESISTOR_PIN, lightReadings);
  
  // Invert the relationship: darker environment = higher brightness
  int mappedBrightness = map(lightLevel, lightCalibrationMin, lightCalibrationMax, 255, 0);
  mainLEDBrightness = constrain(mappedBrightness, 0, 255);
  
  // Optional: Print debug information periodically
  static unsigned long lastDebugTime = 0;
  if (millis() - lastDebugTime > 1000) {
    Serial.print("Auto Mode - Light: ");
    Serial.print(lightLevel);
    Serial.print(", Brightness: ");
    Serial.println(mainLEDBrightness);
    lastDebugTime = millis();
  }
}

// Smooth analog reading using moving average
int smoothAnalogRead(int pin, int readings[]) {
  // Subtract the last reading
  static int total[3] = {0, 0, 0}; // Support for 3 different sensors
  int sensorIndex = (pin == POTENTIOMETER_PIN) ? 0 : (pin == SOUND_SENSOR_PIN) ? 1 : 2;
  
  total[sensorIndex] -= readings[readIndex];
  
  // Read from the sensor
  readings[readIndex] = analogRead(pin);
  
  // Add the reading to the total
  total[sensorIndex] += readings[readIndex];
  
  // Advance to the next position in the array
  if (sensorIndex == 0) { // Only advance index for first sensor to keep sync
    readIndex = (readIndex + 1) % SMOOTHING_SAMPLES;
  }
  
  // Calculate the average
  return total[sensorIndex] / SMOOTHING_SAMPLES;
}

// Calibrate sensors during startup
void calibrateSensors() {
  Serial.println("Calibrating sensors...");
  
  // Calibrate sound threshold
  Serial.println("Calibrating sound sensor - measuring ambient noise...");
  long soundSum = 0;
  for (int i = 0; i < 100; i++) {
    soundSum += analogRead(SOUND_SENSOR_PIN);
    delay(10);
  }
  int ambientNoise = soundSum / 100;
  soundThreshold = ambientNoise + 100; // Set threshold above ambient noise
  
  Serial.print("Ambient noise level: ");
  Serial.print(ambientNoise);
  Serial.print(", Sound threshold set to: ");
  Serial.println(soundThreshold);
  
  // Calibrate light sensor
  Serial.println("Calibrating light sensor - measuring current light levels...");
  long lightSum = 0;
  int minLight = 1023, maxLight = 0;
  
  for (int i = 0; i < 100; i++) {
    int lightReading = analogRead(PHOTORESISTOR_PIN);
    lightSum += lightReading;
    minLight = min(minLight, lightReading);
    maxLight = max(maxLight, lightReading);
    delay(10);
  }
  
  int avgLight = lightSum / 100;
  
  // Set calibration range with some margin
  lightCalibrationMin = max(0, minLight - 50);
  lightCalibrationMax = min(1023, maxLight + 50);
  
  Serial.print("Light calibration - Min: ");
  Serial.print(lightCalibrationMin);
  Serial.print(", Max: ");
  Serial.print(lightCalibrationMax);
  Serial.print(", Average: ");
  Serial.println(avgLight);
  
  Serial.println("Sensor calibration complete!");
}

// Function to manually adjust sound threshold (can be called via serial commands)
void adjustSoundThreshold(int newThreshold) {
  soundThreshold = constrain(newThreshold, 0, 1023);
  Serial.print("Sound threshold adjusted to: ");
  Serial.println(soundThreshold);
}

// Function to manually adjust light calibration (can be called via serial commands)
void adjustLightCalibration(int minVal, int maxVal) {
  lightCalibrationMin = constrain(minVal, 0, 1023);
  lightCalibrationMax = constrain(maxVal, 0, 1023);
  Serial.print("Light calibration adjusted - Min: ");
  Serial.print(lightCalibrationMin);
  Serial.print(", Max: ");
  Serial.println(lightCalibrationMax);
}