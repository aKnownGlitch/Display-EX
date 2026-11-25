# Quick Start Guide

## Project Overview

This is a DCC Monitor Display for model railroad enthusiasts. It connects to a DCC Command Station-EX and displays real-time track status on an ILI9486 touchscreen.

## What You Have

```
Display-Ex/
├── Display-Ex.ino              # Main Arduino sketch (the program)
├── README.md                   # Full project documentation
├── platformio.ini              # PlatformIO build configuration
├── .gitignore                  # Git ignore rules
└── docs/
    ├── PROTOCOL.md             # Serial message format
    └── HARDWARE_SETUP.md       # Wiring diagram and setup
```

## 5-Minute Setup

### 1. Install Arduino IDE (if not already done)
- Download from: https://www.arduino.cc/en/software
- Install Arduino Mega 2560 board package

### 2. Install Required Libraries
In Arduino IDE:
- Sketch → Include Library → Manage Libraries
- Search for and install:
  - **Adafruit GFX Library**
  - **MCUFRIEND_kbv**

### 3. Connect Hardware
1. Wire ILI9486 display to Arduino Mega (see [HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md))
2. Connect USB cable to Arduino Mega
3. Connect DCC Command Station via serial (USB or UART)

### 4. Upload Code
1. Open `Display-Ex.ino` in Arduino IDE
2. Tools → Board → Arduino Mega 2560
3. Tools → Port → (select your Arduino COM port)
4. Click Upload (Ctrl+U)
5. Wait for "Upload successful"

### 5. Test
1. Tools → Serial Monitor (115200 baud)
2. You should see: `DCC Monitor Display initialized`
3. Try sending: `POWER:ON` (and click Send)
4. Display should update

## Sending Status Updates

The Command Station sends messages to the display in this format:

```
POWER:ON              # Track power is on
POWER:OFF             # Track power is off
TURNOUT:1:THROWN      # Turnout 1 is thrown (diverging)
TURNOUT:1:CLOSED      # Turnout 1 is closed (normal)
TURNOUT:2:THROWN
TURNOUT:2:CLOSED
TURNOUT:3:THROWN
TURNOUT:3:CLOSED
THROTTLE:3:0          # Loco 3 at 0 speed
THROTTLE:3:128        # Loco 3 at half speed
THROTTLE:3:255        # Loco 3 at full speed
```

**From Command Station-EX**, you would typically call a function like:

```cpp
Serial.println("POWER:ON");
Serial.println("THROTTLE:3:150");
```

## Display Layout

```
╔════════════════════════════════╗
║      DCC Monitor               ║
╠════════════════════════════════╣
║ Track Power:              [ON] ║   (Green if ON, Red if OFF)
║                                ║
║ Turnouts:                      ║
║   T1: CLOSED                   ║   (Cyan=CLOSED, Magenta=THROWN)
║   T2: CLOSED                   ║
║   T3: CLOSED                   ║
║                                ║
║ Loco 3 Throttle:         150/255║
║ ████████████░░░░░░░░░░░░░░░░░░  ║   (Visual throttle bar)
╚════════════════════════════════╝
```

## Customizing the Code

### Change display rotation (portrait/landscape)
In `Display-Ex.ino`, find this line and change the value:
```cpp
tft.setRotation(1);  // 0=Portrait, 1=Landscape, 2=Portrait flip, 3=Landscape flip
```

### Change update interval
```cpp
#define UPDATE_INTERVAL 1000  // milliseconds (currently 1 second)
```

### Monitor a different locomotive
In `processCommand()`, change `THROTTLE:3` to `THROTTLE:5` (for example) to monitor locomotive 5 instead.

### Add more turnouts
Follow the pattern in `processCommand()` for TURNOUT:1-3 and add:
```cpp
else if (strncmp(command, "TURNOUT:4:", 10) == 0) {
  status.turnout4 = strcmp(&command[10], "THROWN") == 0;
}
```

Then add display code similar to the existing turnouts.

## Troubleshooting

| Issue | Solution |
|-------|----------|
| **Display not lighting up** | Check power connections, verify 5V to VCC pins |
| **Display shows garbage** | Try different rotation values (0, 1, 2, 3) |
| **Serial messages not received** | Verify baud rate is 115200, check TX/RX connections |
| **Compile errors** | Verify MCUFRIEND_kbv and Adafruit GFX libraries are installed |
| **Display ID not found** | Run MCUFRIEND diagnostic sketch to identify display type |

See full troubleshooting in [HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md)

## Next Steps

### Immediate
1. ✅ Set up hardware and upload code
2. ✅ Test serial communication
3. ✅ Verify display is working

### Short Term
- [ ] Integrate with DCC Command Station-EX
- [ ] Send real status updates from command station
- [ ] Tune display colors and layout to your preference
- [ ] Test with actual track power and turnouts

### Future Enhancements
- [ ] Graphical track schematic display
- [ ] Locomotive animation
- [ ] Touch screen controls
- [ ] Additional locomotives and sensors
- [ ] SD card logging

## Getting Help

- **Arduino IDE Help**: https://www.arduino.cc/en/Guide
- **MCUFRIEND Library**: https://github.com/prenticedavid/MCUFRIEND_kbv
- **Adafruit GFX**: https://learn.adafruit.com/adafruit-gfx-graphics-library
- **DCC Info**: https://www.nmra.org/

## Important Files

- [README.md](README.md) - Full project documentation
- [docs/PROTOCOL.md](docs/PROTOCOL.md) - Serial protocol details
- [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md) - Wiring and setup

---

**Last Updated**: 2026-11-25
**Project Version**: 1.0 (Initial Release)
