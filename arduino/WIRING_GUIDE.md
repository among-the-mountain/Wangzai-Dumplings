# Wiring Guide - Multi-Mode Adaptive Lighting Control System

## Complete Wiring Diagram

```
                    ┌─────────────────────────────────────┐
                    │           Arduino Uno               │
                    │                                     │
    5V ─────────────┤ 5V                              A0 ├──── Potentiometer (Center)
                    │                                 A1 ├──── Sound Sensor (OUT)
    GND ────────────┤ GND                             A2 ├──── LDR (one terminal)
                    │                                     │
           ┌────────┤ D2 (INT0)                       D3 ├──── Green LED (220Ω) ──── GND
           │        │                                 D4 ├──── Red LED (220Ω) ──── GND
    Button ┴        │                                 D5 ├──── Yellow LED (220Ω) ──── GND
    (10kΩ to GND)   │                                     │
                    │                                 D9 ├──── Main LED (220Ω) ──── GND
                    └─────────────────────────────────────┘
                                     │
                    ┌────────────────┼────────────────┐
                    │                                 │
              Potentiometer                      Sound Sensor
             ┌─────────────┐                   ┌─────────────┐
    5V ──────┤ 1       3 ├──── GND            │    VCC ├──── 5V
             │             │                  │    GND ├──── GND
    A0 ──────┤ 2 (wiper)   │                  │    OUT ├──── A1
             └─────────────┘                   └─────────────┘

                    Photoresistor (LDR) Circuit
                           ┌─────────────┐
              5V ──────────┤             │
                           │     LDR     ├──── A2 ──── 10kΩ ──── GND
                           │             │
                           └─────────────┘
```

## Step-by-Step Wiring Instructions

### 1. Power Connections
```
Arduino 5V  → Breadboard positive rail (+)
Arduino GND → Breadboard negative rail (-)
```

### 2. Main LED (PWM Controlled)
```
Arduino D9 → 220Ω Resistor → LED Anode (longer leg)
LED Cathode (shorter leg) → GND
```

### 3. Status LEDs
```
Green LED (Manual Mode):
Arduino D3 → 220Ω Resistor → Green LED Anode → GND

Red LED (Sound Mode):
Arduino D4 → 220Ω Resistor → Red LED Anode → GND

Yellow LED (Auto Mode):
Arduino D5 → 220Ω Resistor → Yellow LED Anode → GND
```

### 4. Button with Pull-down Resistor
```
Button Terminal 1 → Arduino D2
Button Terminal 2 → 5V
Arduino D2 → 10kΩ Resistor → GND
```

### 5. Potentiometer (Manual Control)
```
Potentiometer Terminal 1 → 5V
Potentiometer Terminal 2 (center/wiper) → Arduino A0
Potentiometer Terminal 3 → GND
```

### 6. Sound Sensor Module
```
Sound Sensor VCC → 5V
Sound Sensor GND → GND  
Sound Sensor OUT → Arduino A1
```

### 7. Photoresistor (LDR) Circuit
```
5V → LDR → Arduino A2
Arduino A2 → 10kΩ Resistor → GND
```

## Component Specifications

### Required Components

| Component | Specification | Quantity | Notes |
|-----------|---------------|----------|-------|
| Arduino Uno | ATmega328P based | 1 | Arduino Nano also compatible |
| Breadboard | Half-size minimum | 1 | For prototyping |
| LEDs | 5mm, any colors | 4 | 1 main + 3 status |
| Resistors | 220Ω, 1/4W | 4 | LED current limiting |
| Resistors | 10kΩ, 1/4W | 2 | Pull-down + LDR |
| Potentiometer | 10kΩ linear | 1 | Rotary or slide |
| Push Button | Momentary SPST | 1 | Tactile switch |
| Sound Sensor | Analog output | 1 | KY-037 or similar |
| Photoresistor | 5-10kΩ @ 25°C | 1 | GL5516 or similar |
| Jumper Wires | Male-to-male | 20+ | Various lengths |

### Recommended Modules

**Sound Sensor Options:**
- KY-037 Sound Detection Sensor
- LM393 Sound Sensor Module
- MAX4466 Microphone Amplifier

**Photoresistor Options:**
- GL5516 (5-10kΩ range)
- GL5537 (10-20kΩ range)
- GL5528 (5-10kΩ range)

## Circuit Analysis

### Power Distribution
- Total current draw: ~100mA typical
- Arduino: ~20mA
- LEDs: 4 × 20mA = 80mA maximum
- Sensors: <5mA combined

### Voltage Levels
- Digital HIGH: 5V
- Digital LOW: 0V
- Analog Reference: 5V (1024 steps = 4.88mV/step)
- PWM Output: 0-5V (256 levels)

### Pin Usage Summary
```
Digital Pins:
- D2: Button input (interrupt capable)
- D3: Green LED output
- D4: Red LED output  
- D5: Yellow LED output
- D9: Main LED PWM output

Analog Pins:
- A0: Potentiometer input
- A1: Sound sensor input
- A2: Photoresistor input
```

## Testing Procedures

### 1. Power Test
1. Connect only power (5V, GND)
2. Check voltage at power rails
3. Verify Arduino powers on (LED13 should blink during boot)

### 2. LED Test
1. Upload basic blink sketch
2. Test each LED individually
3. Verify proper brightness and no flickering

### 3. Button Test
1. Upload simple button reading sketch
2. Test single press, double press, long press
3. Verify clean signal without bouncing

### 4. Sensor Test
1. Read analog values from each sensor
2. Potentiometer: 0-1023 range when rotated
3. Sound sensor: Changes with audio input
4. Photoresistor: Changes with light level

### 5. Integration Test
1. Upload main sketch
2. Test mode switching
3. Verify each mode operates correctly
4. Check serial output for debug info

## Troubleshooting Guide

### Common Issues

**LEDs not lighting:**
- Check polarity (anode to pin, cathode to GND)
- Verify resistor values (too high = dim, too low = damage)
- Test with multimeter for continuity

**Button not responding:**
- Check pull-down resistor (10kΩ)
- Verify connection to interrupt pin (D2)
- Test button with multimeter

**Erratic sensor readings:**
- Check analog reference voltage
- Ensure stable power supply
- Add capacitors for noise filtering if needed

**Mode not switching:**
- Monitor serial output for debug info
- Check button timing constants in code
- Verify interrupt setup

### Debugging Steps

1. **Serial Monitor**: Always check serial output first
2. **Multimeter**: Verify voltages and continuity
3. **Oscilloscope**: For advanced signal analysis
4. **Component Swap**: Replace suspected faulty parts

## Safety Considerations

### Electrical Safety
- Never exceed component ratings
- Use appropriate resistors for LED current limiting
- Double-check polarity before applying power
- Disconnect power when making wiring changes

### Component Protection
- LEDs: Always use current-limiting resistors
- Arduino: Don't exceed 40mA per pin, 200mA total
- Sensors: Follow manufacturer specifications
- Power Supply: Use regulated 5V source

## Advanced Wiring Options

### PCB Layout Considerations
For permanent installation:
- Keep analog traces away from digital switching
- Use ground plane for noise reduction
- Add bypass capacitors near ICs
- Consider EMI shielding for sensitive circuits

### Wire Management
- Use different colors for different signal types
- Keep wires as short as practical
- Avoid parallel runs of analog and digital signals
- Secure connections with appropriate connectors

### Expansion Options
The design allows for future enhancements:
- Additional analog inputs (A3-A5 available)
- I2C devices (A4=SDA, A5=SCL)
- SPI communication (D10-D13)
- Additional digital I/O (D6-D8, D10-D13)

This wiring guide provides complete instructions for building the Multi-Mode Adaptive Lighting Control System. Follow the steps carefully and test each component before final assembly.