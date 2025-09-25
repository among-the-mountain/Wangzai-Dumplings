# Circuit Diagram - Multi-Mode Adaptive Lighting Control System

## Complete Circuit Schematic

```
                              Multi-Mode Adaptive Lighting Control System
                                           Circuit Diagram

                    +5V ────────────────────────┬─────────────────────────────────────┐
                                                │                                     │
                                                │                                     │
                    ┌───────────────────────────┼─────────────────────────────────────┼──┐
                    │                           │                                     │  │
                    │        Arduino Uno        │                                     │  │
                    │                           │                                     │  │
                    │                      ┌────┴────┐                              │  │
                    │                 5V ──┤         │                              │  │
                    │                GND ──┤         │                              │  │
                    │                      │         │                              │  │
                    │                 D2 ──┤         ├── A0                        │  │
                    │                 D3 ──┤         ├── A1                        │  │
                    │                 D4 ──┤         ├── A2                        │  │
                    │                 D5 ──┤         │                              │  │
                    │                 D9 ──┤         │                              │  │
                    │                      └─────────┘                              │  │
                    └─────────────────────────────────────────────────────────────────┘  │
                                                                                         │
    ┌─── Status LEDs with Current Limiting Resistors ───┐                             │
    │                                                   │                             │
    │   Green LED (Manual Mode)     Red LED (Sound)     │    Yellow LED (Auto)       │
    │   D3 ──[220Ω]──├──┤  GND      D4 ──[220Ω]──├──┤  │    D5 ──[220Ω]──├──┤  GND  │
    │                                                   │                             │
    └───────────────────────────────────────────────────┘                             │
                                                                                      │
    ┌─── Main LED (PWM Controlled) ───┐                                               │
    │                                 │                                               │
    │   Main LED                      │                                               │
    │   D9 ──[220Ω]──├──┤  GND        │                                               │
    │                                 │                                               │
    └─────────────────────────────────┘                                               │
                                                                                      │
    ┌─── Button with Pull-down Resistor ───┐                                          │
    │                                      │                                          │
    │   Push Button                        │                                          │
    │   +5V ──┤   ┝── D2 ──[10kΩ]── GND   │                                          │
    │                                      │                                          │
    └──────────────────────────────────────┘                                          │
                                                                                      │
    ┌─── Potentiometer (Manual Control) ───┐                                          │
    │                                      │                                          │
    │         10kΩ Potentiometer           │                                          │
    │   +5V ──┤1     2├── A0               │                                          │
    │              3├── GND                │                                          │
    │                                      │                                          │
    └──────────────────────────────────────┘                                          │
                                                                                      │
    ┌─── Sound Sensor Module ───┐                                                     │
    │                           │                                                     │
    │    KY-037 Sound Sensor    │                                                     │
    │    VCC ── +5V             │                                                     │
    │    GND ── GND             │                                                     │
    │    OUT ── A1              │                                                     │
    │                           │                                                     │
    └───────────────────────────┘                                                     │
                                                                                      │
    ┌─── Photoresistor (LDR) with Voltage Divider ───┐                               │
    │                                                 │                               │
    │    Light Dependent Resistor                     │                               │
    │    +5V ──┤LDR├── A2 ──[10kΩ]── GND             │                               │
    │                                                 │                               │
    └─────────────────────────────────────────────────┘                               │
                                                                                      │
    GND ──────────────────────────────────────────────────────────────────────────────┘
```

## Pin Assignment Summary

| Pin | Component | Function | Notes |
|-----|-----------|----------|-------|
| **Power** |
| 5V | Power Rail | +5V Supply | Connect to breadboard + rail |
| GND | Ground Rail | Ground | Connect to breadboard - rail |
| **Digital Outputs** |
| D3 | Green LED | Manual Mode Indicator | Via 220Ω resistor |
| D4 | Red LED | Sound Mode Indicator | Via 220Ω resistor |
| D5 | Yellow LED | Auto Mode Indicator | Via 220Ω resistor |
| D9 | Main LED | PWM Brightness Control | Via 220Ω resistor |
| **Digital Input** |
| D2 | Button | Mode Switch Input | With 10kΩ pull-down |
| **Analog Inputs** |
| A0 | Potentiometer | Manual Brightness Control | Center tap, 0-5V |
| A1 | Sound Sensor | Audio Level Detection | Analog output |
| A2 | Photoresistor | Light Level Detection | Voltage divider |

## Component Values and Specifications

### Resistors
```
LED Current Limiting: 220Ω (1/4W)
- Calculation: (5V - 2V) / 0.015A ≈ 200Ω (220Ω standard)
- Provides ~13.6mA LED current (safe operating range)

Pull-down Resistor: 10kΩ (1/4W)
- Ensures button pin reads LOW when not pressed
- High enough to not waste power, low enough for reliable operation

LDR Voltage Divider: 10kΩ (1/4W)
- Forms voltage divider with photoresistor
- Provides 0-5V range for various lighting conditions
```

### LEDs
```
Forward Voltage: ~2.0V (typical for standard LEDs)
Forward Current: 15-20mA (with 220Ω resistor)
Colors: Red, Green, Yellow, plus one main LED (any color)
```

### Sensors
```
Potentiometer: 10kΩ Linear Taper
- Provides smooth 0-5V output for brightness control
- Linear response for intuitive feel

Sound Sensor: KY-037 or equivalent
- Operating Voltage: 3.3V - 5V
- Output: Analog 0-5V proportional to sound level
- Built-in amplification and filtering

Photoresistor: GL5516 (typical)
- Dark Resistance: ~1MΩ
- Light Resistance: ~10kΩ
- Response Time: ~20ms
```

## Breadboard Layout Suggestion

```
     1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
   ┌────────────────────────────────────────────────────────────────────────────────────────┐
 a │ +  +  +  +  +     +  +  +  +  +     +  +  +  +  +     +  +  +  +  +     +  +  +  +  + │ +5V
 b │ +  +  +  +  +     +  +  +  +  +     +  +  +  +  +     +  +  +  +  +     +  +  +  +  + │
   ├────────────────────────────────────────────────────────────────────────────────────────┤
 c │    [Arduino Uno positioned here spanning multiple rows]                                 │
 d │                                                                                        │
 e │  G  G  Y  R  M     P  P  P        S  S  S        L  L        B  B                     │
 f │  r  r  e  e  a     o  o  o        o  o  o        D  D        u  u                     │
 g │  n  n  l  d  i     t  t  t        u  u  u        R  R        t  t                     │
 h │              n     1  2  3        V  G  O              10k   1  2                     │
 i │              L                     C  N  U              R     |  |                     │
 j │              D                     C  D  T              e     |  +5V                   │
   ├────────────────────────────────────────────────────────────────────────────────────────┤
 k │ -  -  -  -  -     -  -  -  -  -     -  -  -  -  -     -  -  -  -  -     -  -  -  -  - │ GND
 l │ -  -  -  -  -     -  -  -  -  -     -  -  -  -  -     -  -  -  -  -     -  -  -  -  - │
   └────────────────────────────────────────────────────────────────────────────────────────┘

Legend:
Grn = Green LED + 220Ω resistor
Yel = Yellow LED + 220Ω resistor  
Red = Red LED + 220Ω resistor
Main = Main LED + 220Ω resistor
Pot1/2/3 = Potentiometer terminals
Sou VCC/GND/OUT = Sound sensor connections
LDR = Photoresistor + 10kΩ resistor
But1/2 = Button terminals
```

## Power Analysis

### Current Consumption
```
Arduino Uno Base:        ~20 mA
Main LED (max):          ~15 mA  
Status LEDs (1 active):  ~15 mA
Sound Sensor:            ~2 mA
Other components:        ~3 mA
                        --------
Total Maximum:           ~55 mA
Typical Operation:       ~40 mA
```

### Power Supply Requirements
- **Voltage**: 5V DC regulated
- **Current**: 100mA minimum (for safety margin)
- **Source**: USB (500mA available) or 7-12V wall adapter

## Signal Analysis

### Analog Signal Ranges
```
Potentiometer (A0):    0V to 5V (0 to 1023 ADC)
Sound Sensor (A1):     0V to 5V (varies with audio)
Photoresistor (A2):    0V to 5V (varies with light)
```

### Digital Signal Levels
```
Button Input (D2):     0V (LOW) or 5V (HIGH)
LED Outputs (D3-D5):   0V (OFF) or 5V (ON)
Main LED (D9):         0V to 5V PWM (0-255 duty)
```

### Timing Characteristics
```
Button Debounce:       50ms
Long Press Detect:     2000ms
Double Click Window:   300ms
Sensor Update Rate:    ~100Hz
PWM Frequency:         490Hz
```

## Safety and Design Notes

### Component Protection
- All LEDs protected by current-limiting resistors
- Button has pull-down resistor to prevent floating inputs
- Sensors operate within voltage specifications
- No components exceed Arduino pin current limits

### Signal Integrity  
- Analog lines kept away from switching digital signals
- Power and ground distributed via breadboard rails
- Short wire runs minimize noise and interference
- Bypass capacitors may be added for improved stability

### Reliability Features
- Hardware debouncing through software timing
- Sensor smoothing via moving average filtering
- Auto-calibration adapts to environmental conditions
- Interrupt-driven button handling for responsiveness

This circuit provides a robust, reliable implementation of the multi-mode adaptive lighting control system with proper component protection and signal conditioning.