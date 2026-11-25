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

### 1. Set Up Arduino in VS Code
If you haven't already:
- Install **Arduino Maker Workshop** extension (search in VS Code Extensions)
- The extension will guide you through Arduino setup on first use
- It will detect your Arduino installation and board packages

### 2. Open the Project in VS Code
1. File → Open Folder
2. Navigate to and select: `/Volumes/Extreme Pro/src/arduino/Display-Ex`
3. VS Code will recognize this as an Arduino project

### 3. Install Required Libraries
In VS Code:
- Click the Arduino icon in the sidebar (Maker Workshop)
- Navigate to Library Manager
- Search for and install:
  - **Adafruit GFX Library**
  - **MCUFRIEND_kbv**
- Or use: `Command Palette (Cmd+Shift+P) → Arduino: Manage Libraries`

### 4. Configure Board & Port
1. Click Arduino icon in sidebar → Board Manager
2. Select Board: **Arduino Mega 2560**
3. Select Port: **(your USB COM port)**
4. Alternatively use Command Palette:
   - `Arduino: Select Board` 
   - `Arduino: Select Serial Port`

### 5. Connect Hardware
1. Wire ILI9486 display to Arduino Mega (see [HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md))
2. Connect USB cable to Arduino Mega to your computer
3. Connect DCC Command Station via serial (USB or UART)

### 6. Build and Upload
1. Click Arduino icon in sidebar → Compile
   - Or use: `Command Palette → Arduino: Verify/Compile`
   - Wait for "Build complete"
2. Click Arduino icon → Upload 
   - Or use: `Command Palette → Arduino: Upload`
   - Wait for "Upload complete"

### 7. Test
1. Click Arduino icon → Open Serial Monitor
   - Or use: `Command Palette → Arduino: Open Serial Monitor`
   - Set baud rate to **115200**
2. You should see: `DCC Monitor Display initialized`
3. Try sending: `POWER:ON` (type and press Enter)
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

## VS Code Arduino Maker Workshop Tips

### Quick Commands (Use Command Palette: Cmd+Shift+P)
- `Arduino: Verify/Compile` - Check code without uploading
- `Arduino: Upload` - Compile and upload to board
- `Arduino: Open Serial Monitor` - Monitor serial output
- `Arduino: Select Board` - Change target board
- `Arduino: Select Serial Port` - Change USB port
- `Arduino: Manage Libraries` - Install/remove libraries

### Keyboard Shortcuts
- `Cmd+Shift+P` - Open Command Palette
- `Cmd+B` - Build/Compile
- `Cmd+K Cmd+V` - Open Preview for Markdown files

### Status Bar
- Bottom of VS Code shows:
  - Current board selection
  - Selected serial port
  - Upload/Compile status

### Troubleshooting in VS Code
- Open **Output** panel (View → Output) to see detailed compilation messages
- Check **Problems** panel (View → Problems) for syntax errors
- Use **Serial Monitor** to debug by sending test messages

### Setting Up Code Completion
The Arduino Maker Workshop extension provides IntelliSense for:
- Arduino built-in functions
- Installed libraries
- Custom functions and variables

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

**VS Code & Arduino Maker Workshop:**
- Arduino Maker Workshop Extension Documentation (in VS Code Help)
- Arduino Official: https://www.arduino.cc/
- VS Code Arduino Support: https://code.visualstudio.com/docs/other/arduino

**Arduino Libraries & Boards:**
- MCUFRIEND Library: https://github.com/prenticedavid/MCUFRIEND_kbv
- Adafruit GFX: https://learn.adafruit.com/adafruit-gfx-graphics-library
- DCC Info: https://www.nmra.org/

## Important Files

- [README.md](README.md) - Full project documentation
- [docs/PROTOCOL.md](docs/PROTOCOL.md) - Serial protocol details
- [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md) - Wiring and setup

---

**Last Updated**: 2026-11-25
**Project Version**: 1.0 (Initial Release)
