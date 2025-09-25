# Setup Guide - Multi-Mode Adaptive Lighting Control System

## Quick Start Checklist

- [ ] Arduino IDE installed (version 1.8.0 or later)
- [ ] All hardware components gathered (see component list below)
- [ ] Circuit assembled according to wiring diagram
- [ ] Test sketch uploaded and verified
- [ ] Main sketch uploaded and running
- [ ] System calibrated and tested

## Prerequisites

### Software Requirements
- **Arduino IDE**: Version 1.8.0 or later
- **Operating System**: Windows, macOS, or Linux
- **USB Cable**: Type A to Type B (for Arduino Uno)

### Hardware Requirements
Complete component list with specifications:

| Component | Specification | Quantity | Purpose |
|-----------|---------------|----------|---------|
| Arduino Uno/Nano | ATmega328P, 5V, 16MHz | 1 | Main controller |
| Breadboard | 830 tie points minimum | 1 | Circuit assembly |
| Main LED | 5mm, any color, 20mA max | 1 | Primary light output |
| Status LEDs | 5mm Red, Green, Yellow | 3 | Mode indication |
| Potentiometer | 10kΩ, linear taper | 1 | Manual brightness control |
| Push Button | Momentary, SPST | 1 | Mode switching |
| Sound Sensor | KY-037 or equivalent | 1 | Audio detection |
| Photoresistor | GL5516 (5-10kΩ) | 1 | Light sensing |
| Resistors | 220Ω, 1/4W | 4 | LED current limiting |
| Resistors | 10kΩ, 1/4W | 2 | Pull-down, voltage divider |
| Jumper Wires | Various lengths | 20+ | Connections |
| USB Cable | A to B type | 1 | Programming/power |

## Step-by-Step Setup

### Step 1: Install Arduino IDE

1. **Download Arduino IDE**:
   - Visit: https://www.arduino.cc/en/software
   - Download for your operating system
   - Install following standard procedures

2. **Verify Installation**:
   - Open Arduino IDE
   - Check Tools → Board → Arduino Uno is available
   - Check Tools → Port shows available ports

### Step 2: Prepare Hardware

1. **Inventory Components**:
   - Check all components against the list above
   - Verify component specifications
   - Identify component pins and orientations

2. **Prepare Breadboard**:
   - Connect power rails: 5V (red) and GND (black/blue)
   - Test continuity of power rails
   - Organize components on breadboard

### Step 3: Basic Wiring

1. **Power Connections**:
   ```
   Arduino 5V  → Breadboard + rail
   Arduino GND → Breadboard - rail
   ```

2. **Essential Connections First**:
   ```
   Arduino D9  → Main LED (via 220Ω resistor)
   Arduino D3  → Green LED (via 220Ω resistor)
   Arduino D2  → Button (with 10kΩ pull-down)
   Arduino A0  → Potentiometer center pin
   ```

3. **Test Basic Functionality**:
   - Upload blink sketch to verify Arduino works
   - Test LEDs individually
   - Verify button and potentiometer readings

### Step 4: Complete Wiring

Follow the complete wiring diagram to connect:
- All status LEDs (Red D4, Yellow D5)
- Sound sensor (A1)
- Photoresistor with voltage divider (A2)
- All ground and power connections

**Detailed wiring steps in WIRING_GUIDE.md**

### Step 5: Software Upload

1. **Component Testing**:
   ```
   File → Open → test_components.ino
   Upload to Arduino
   Open Serial Monitor (9600 baud)
   Test each component individually
   ```

2. **Main Program**:
   ```
   File → Open → adaptive_lighting_control.ino
   Verify code compiles without errors
   Upload to Arduino
   ```

### Step 6: System Verification

1. **Power-On Test**:
   - Arduino should start in Manual Mode (Green LED on)
   - Serial Monitor shows initialization messages
   - System performs automatic calibration

2. **Mode Testing**:
   - **Single Click**: Should switch to Sound Control Mode (Red LED)
   - **Double Click**: Should return to Manual Mode (Green LED)
   - **Long Press**: Should enter Automatic Mode (Yellow LED)

3. **Functionality Testing**:
   - **Manual Mode**: Potentiometer controls brightness
   - **Sound Mode**: Loud sounds trigger full brightness
   - **Auto Mode**: Covering LDR increases brightness

## Calibration Procedures

### Automatic Calibration (Default)
The system performs automatic calibration on startup:
- Sound threshold set above ambient noise level
- Light sensor range measured and stored
- Calibration results displayed in Serial Monitor

### Manual Calibration (Optional)

1. **Sound Threshold Adjustment**:
   ```cpp
   // In Serial Monitor, you can call:
   adjustSoundThreshold(newValue); // 0-1023
   ```

2. **Light Sensor Range**:
   ```cpp
   // Adjust light calibration range:
   adjustLightCalibration(minValue, maxValue);
   ```

3. **Interactive Calibration**:
   - Use test_components.ino
   - Run printCalibrationData() function
   - Note min/max values for each sensor
   - Update main code with measured values

## Troubleshooting Common Issues

### Arduino Not Recognized
**Symptoms**: IDE doesn't show Arduino port
**Solutions**:
- Check USB cable (data cable, not charge-only)
- Install Arduino drivers (Windows)
- Try different USB port
- Restart Arduino IDE

### Compilation Errors
**Symptoms**: Sketch won't compile
**Solutions**:
- Check Arduino IDE version (1.8.0+)
- Verify board selection (Tools → Board → Arduino Uno)
- Check for typos in pin definitions
- Ensure all libraries are available

### LEDs Not Working
**Symptoms**: LEDs don't light up or are very dim
**Solutions**:
- Check LED polarity (long leg = anode = positive)
- Verify resistor values (220Ω recommended)
- Test LEDs with multimeter
- Check power supply voltage (should be 5V)

### Button Not Responding
**Symptoms**: Mode switching doesn't work
**Solutions**:
- Verify pull-down resistor (10kΩ to GND)
- Check button wiring to pin D2
- Test button with multimeter
- Monitor Serial output for button press messages

### Erratic Sensor Readings
**Symptoms**: Sensors give inconsistent values
**Solutions**:
- Check analog pin connections
- Verify power supply stability
- Allow time for sensor smoothing to stabilize
- Check for loose connections

### Serial Monitor Issues
**Symptoms**: No output or garbled text
**Solutions**:
- Set baud rate to 9600
- Check Arduino port selection
- Try closing/reopening Serial Monitor
- Verify Arduino is running (LED13 should be active)

## Performance Optimization

### Response Time Tuning
Adjust these parameters in the code:
```cpp
const unsigned long DEBOUNCE_DELAY = 50;      // Button debounce
const unsigned long LONG_PRESS_TIME = 2000;   // Long press duration
const unsigned long DOUBLE_CLICK_TIME = 300;  // Double-click window
```

### Sensor Smoothing
Modify smoothing for different response characteristics:
```cpp
const int SMOOTHING_SAMPLES = 10; // More = smoother, slower response
```

### Sound Sensitivity
Adjust sound detection sensitivity:
```cpp
const unsigned long SOUND_ACTIVE_TIME = 3000; // Duration of activation
// soundThreshold is auto-calibrated but can be manually adjusted
```

## Advanced Configuration

### Custom Pin Assignments
To use different pins, modify these definitions:
```cpp
const int MAIN_LED_PIN = 9;        // Must be PWM capable
const int BUTTON_PIN = 2;          // Must be interrupt capable (2 or 3)
const int GREEN_LED_PIN = 3;       // Any digital pin
// ... etc
```

### PWM Frequency Adjustment
For different LED characteristics:
```cpp
// Add to setup() for custom PWM frequency
TCCR1B = TCCR1B & B11111000 | B00000001; // 31372 Hz
```

### Power Management
For battery operation, add sleep modes:
```cpp
#include <avr/sleep.h>
// Implement sleep between operations
```

## Maintenance and Updates

### Regular Maintenance
- Check connections periodically
- Clean sensors (especially photoresistor)
- Verify power supply stability
- Update calibration if environment changes

### Software Updates
- Keep Arduino IDE updated
- Check for code improvements
- Document any custom modifications
- Backup working configurations

## Safety Guidelines

### Electrical Safety
- Always disconnect power when modifying circuits
- Use appropriate resistors for LED current limiting
- Don't exceed component current ratings
- Double-check polarity before connecting power

### Component Protection
- Handle static-sensitive components properly
- Don't reverse power connections
- Use heat sinks for high-power LEDs if needed
- Keep circuits clean and dry

## Getting Help

### Documentation Resources
- Arduino Reference: https://www.arduino.cc/reference/
- Component datasheets (check manufacturer websites)
- Breadboard tutorials for beginners

### Community Support
- Arduino Forums: https://forum.arduino.cc/
- Reddit: r/arduino
- Local maker spaces and Arduino user groups

### Project-Specific Help
1. Check Serial Monitor output first
2. Review wiring diagram carefully
3. Test components individually
4. Document any modifications made

## Next Steps

Once your system is working:
- Experiment with different sensors
- Add WiFi connectivity
- Implement scheduling features
- Create custom enclosure
- Share your improvements with the community

This setup guide should get you from components to working system. Take your time with each step and don't hesitate to test frequently!