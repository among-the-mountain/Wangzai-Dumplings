# Multi-Mode Adaptive Lighting Control System

This Arduino project implements a sophisticated multi-mode lighting control system that can operate in three different modes with intelligent mode switching capabilities.

## Features

### Operating Modes

1. **Manual Mode (Default)**
   - Uses a potentiometer to manually control LED brightness (0-100%)
   - Real-time brightness adjustment with smooth analog control
   - Indicated by a **Green LED**
   - Default mode on system startup

2. **Sound Control Mode**
   - Activates when ambient sound exceeds a configurable threshold
   - Brightness jumps to 100% when sound is detected
   - Gradually fades back to minimum after 3 seconds of silence
   - Indicated by a **Red LED**
   - Auto-calibrates sound threshold based on ambient noise

3. **Automatic Light Control Mode**
   - Uses a photoresistor (LDR) to automatically adjust brightness
   - Darker environment = Higher brightness (inverse relationship)
   - Continuous adaptive adjustment based on ambient light
   - Indicated by a **Yellow LED**
   - Auto-calibrates light sensor range during startup

### Mode Switching

- **Single Click**: Enter Sound Control Mode
- **Double Click**: Return to Manual Mode (from any mode)
- **Long Press (>2 seconds)**: Enter Automatic Light Control Mode

### Advanced Features

- **Button Debouncing**: Prevents false triggers and ensures reliable operation
- **Sensor Smoothing**: Uses moving average filtering for stable readings
- **Auto-Calibration**: Automatically adjusts sensor thresholds during startup
- **Serial Debugging**: Comprehensive logging and real-time monitoring
- **Interrupt-Based Button Handling**: Responsive and efficient button detection

## Hardware Requirements

### Components List

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Uno/Nano | 1 | Main microcontroller |
| Main LED (any color) | 1 | Primary controllable light |
| Potentiometer (10kΩ) | 1 | Manual brightness control |
| Push Button | 1 | Mode switching |
| Pull-down Resistor (10kΩ) | 1 | Button debouncing |
| Sound Sensor Module | 1 | Sound detection |
| Photoresistor (LDR) | 1 | Light level detection |
| Resistor for LDR (10kΩ) | 1 | Voltage divider |
| Status LEDs (Red, Green, Yellow) | 3 | Mode indication |
| LED Resistors (220Ω-330Ω) | 4 | Current limiting for LEDs |
| Breadboard/PCB | 1 | Circuit assembly |
| Jumper Wires | Various | Connections |

### Pin Connections

```
Arduino Pin  | Component
-------------|------------------
Pin 9 (PWM)  | Main LED (Anode)
Pin 3        | Green LED (Manual Mode)
Pin 4        | Red LED (Sound Mode)
Pin 5        | Yellow LED (Auto Mode)
Pin 2        | Button (Interrupt Pin)
A0           | Potentiometer (Center Pin)
A1           | Sound Sensor (Analog Out)
A2           | Photoresistor (Voltage Divider)
5V           | Power Supply (+)
GND          | Ground (-)
```

## Wiring Diagram

```
                    Arduino Uno
                   ┌─────────────┐
                   │             │
    ┌─ 5V ─────────┤ 5V      A0 ├──── Potentiometer (Center)
    │              │         A1 ├──── Sound Sensor (OUT)
    │  ┌─ GND ─────┤ GND     A2 ├──── Photoresistor
    │  │           │         D2 ├──── Button
    │  │           │         D3 ├──── Green LED
    │  │           │         D4 ├──── Red LED  
    │  │           │         D5 ├──── Yellow LED
    │  │           │         D9 ├──── Main LED (PWM)
    │  │           └─────────────┘
    │  │
    └──┼─ Potentiometer (5V)
       │
       └─ Potentiometer (GND)
```

### Detailed Wiring Instructions

1. **Main LED Circuit**:
   - Connect main LED anode to Arduino Pin 9 through 220Ω resistor
   - Connect cathode to GND

2. **Status LEDs**:
   - Green LED: Anode to Pin 3, Cathode to GND (via 220Ω resistor)
   - Red LED: Anode to Pin 4, Cathode to GND (via 220Ω resistor)
   - Yellow LED: Anode to Pin 5, Cathode to GND (via 220Ω resistor)

3. **Button Circuit**:
   - One terminal to Pin 2
   - Other terminal to 5V
   - 10kΩ pull-down resistor from Pin 2 to GND

4. **Potentiometer**:
   - Terminal 1: 5V
   - Terminal 2 (center): A0
   - Terminal 3: GND

5. **Sound Sensor**:
   - VCC to 5V
   - GND to GND
   - OUT to A1

6. **Photoresistor (LDR)**:
   - One terminal to 5V
   - Other terminal to A2 and 10kΩ resistor
   - 10kΩ resistor other terminal to GND

## Installation & Setup

### 1. Hardware Assembly
1. Assemble the circuit according to the wiring diagram
2. Double-check all connections before powering on
3. Ensure proper resistor values for LED current limiting

### 2. Software Upload
1. Open Arduino IDE
2. Load the `adaptive_lighting_control.ino` file
3. Select your Arduino board type and port
4. Upload the sketch to your Arduino

### 3. System Initialization
1. Open Serial Monitor (9600 baud rate)
2. System will automatically calibrate sensors on startup
3. Watch for calibration messages and verify proper operation

## Usage Instructions

### Getting Started
1. Power on the system - it starts in Manual Mode (Green LED on)
2. Use the potentiometer to adjust brightness manually
3. Press the button to switch between modes

### Mode Operations

**Manual Mode (Green LED)**:
- Turn potentiometer clockwise to increase brightness
- Turn counterclockwise to decrease brightness
- Real-time brightness control with smooth response

**Sound Control Mode (Red LED)**:
- Make noise (clap, speak, play music) to trigger full brightness
- LED will maintain full brightness for 3 seconds after sound
- Gradual fade-out after silence period
- Threshold auto-adjusts to ambient noise level

**Automatic Mode (Yellow LED)**:
- Cover photoresistor to simulate darkness - brightness increases
- Shine light on photoresistor - brightness decreases
- Continuous adjustment based on ambient light conditions

### Troubleshooting

**Button not responding**:
- Check button wiring and pull-down resistor
- Verify connection to interrupt pin (Pin 2)

**LEDs not working**:
- Check LED polarity (anode to pin, cathode to GND)
- Verify resistor values (220-330Ω recommended)

**Sensors not calibrating**:
- Ensure proper analog pin connections
- Check power supply connections (5V and GND)
- Monitor Serial output for calibration messages

**Inconsistent readings**:
- System uses smoothing algorithms - allow a few seconds for stabilization
- Check for loose connections
- Ensure stable power supply

## Advanced Features

### Serial Commands
The system supports real-time monitoring via Serial Monitor:
- Real-time sensor readings
- Mode change notifications
- Calibration status updates
- Debug information

### Customization Options
You can modify these parameters in the code:
- `LONG_PRESS_TIME`: Duration for long press detection (default: 2000ms)
- `DOUBLE_CLICK_TIME`: Window for double-click detection (default: 300ms)
- `SOUND_ACTIVE_TIME`: Duration of full brightness after sound (default: 3000ms)
- `SMOOTHING_SAMPLES`: Number of samples for sensor smoothing (default: 10)

### Calibration Functions
Manual calibration functions available:
- `adjustSoundThreshold(int newThreshold)`: Set custom sound threshold
- `adjustLightCalibration(int minVal, int maxVal)`: Set custom light range

## Technical Specifications

- **Operating Voltage**: 5V DC
- **Current Consumption**: ~100mA (typical)
- **Response Time**: <50ms for mode switching
- **Sensor Update Rate**: 100Hz
- **PWM Frequency**: 490Hz (Arduino default)
- **Button Debounce**: 50ms
- **Sensor Smoothing**: 10-sample moving average

## Code Structure

The code is organized into several key functions:

- `setup()`: System initialization and calibration
- `loop()`: Main execution loop
- `handleButtonPress()`: Advanced button handling with debouncing
- `setMode()`: Mode switching and LED status updates
- `executeManualMode()`: Manual potentiometer control
- `executeSoundControlMode()`: Sound-triggered lighting
- `executeAutomaticMode()`: Light-sensitive automatic control
- `smoothAnalogRead()`: Sensor noise filtering
- `calibrateSensors()`: Auto-calibration routines

## Future Enhancements

Potential improvements for future versions:
- WiFi connectivity for remote control
- Mobile app integration
- Multiple lighting zones
- Scheduling and timer functions
- Energy usage monitoring
- Advanced sensor fusion algorithms

## Support

For technical support or questions:
1. Check the troubleshooting section
2. Monitor Serial output for debug information
3. Verify all hardware connections
4. Ensure proper component values and orientations

## License

This project is open-source and available for educational and personal use.