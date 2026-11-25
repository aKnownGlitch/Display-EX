# DCC Monitor Display

An Arduino Mega application that monitors DCC (Digital Command Control) command station status and displays it on an ILI9486 touchscreen display.

## Hardware

- **Microcontroller**: Arduino Mega 2560
- **Display**: ILI9486 3.5" TFT LCD Display (480x320)
- **Communication**: Serial (USB/UART) with Command Station-EX

## Monitored Status

The application displays real-time information:
- **Track Power**: ON/OFF status (red/green indicator)
- **Turnout Status**: 3 Turnouts showing THROWN/CLOSED state
- **Throttle Setting**: Speed control (0-255) for Locomotive 3 with visual bar

## Wiring

### Arduino Mega to ILI9486

```
Arduino Pin   ILI9486 Pin   Purpose
-----------   -----------   -------
50 (MISO)     SDO (12)      SPI Data Out
51 (MOSI)     SDI (11)      SPI Data In
52 (SCK)      SCK (13)      SPI Clock
10            CS (10)       Chip Select
9             RS/DC (9)     Register Select / Data/Command
8             WR (8)        Write Enable
7             RD (7)        Read Enable
6             RESET (6)     Reset
GND           GND (1,2)     Ground
5V/3.3V       VCC (3,4)     Power
GND           GND (20)      Backlight Ground
5V            VCC (19)      Backlight Power
```

**Note**: Pin assignments can be modified in the sketch if using different pins.

## Serial Protocol

The display receives status updates from the DCC Command Station via serial (115200 baud) using this message format:

```
POWER:ON       or POWER:OFF
TURNOUT:1:THROWN or TURNOUT:1:CLOSED
TURNOUT:2:THROWN or TURNOUT:2:CLOSED
TURNOUT:3:THROWN or TURNOUT:3:CLOSED
THROTTLE:3:value (where value is 0-255)
```

## Libraries Required

Install via VS Code Arduino Maker Workshop or Arduino IDE Library Manager:

- **Adafruit GFX Library**
- **MCUFRIEND_kbv** (for ILI9486 display)

### Installation in VS Code

1. Click Arduino icon in VS Code sidebar (Maker Workshop)
2. Click Library Manager
3. Search for and install:
   - "Adafruit GFX Library" by Adafruit
   - "MCUFRIEND_kbv" by David Prentice

Or use Command Palette (Cmd+Shift+P):
- `Arduino: Manage Libraries`

### Installation in Arduino IDE

1. Open Arduino IDE → Sketch → Include Library → Manage Libraries
2. Search for and install:
   - "Adafruit GFX Library" by Adafruit
   - "MCUFRIEND_kbv" by David Prentice

Alternatively, in `platformio.ini` for PlatformIO:

```ini
lib_deps =
    adafruit/Adafruit GFX Library @ ^1.11.9
    mcufriend/MCUFRIEND_kbv @ ^5.9.11
```

## Building and Uploading

### Using VS Code Arduino Maker Workshop

1. Open the project folder in VS Code
2. Click Arduino icon in sidebar to access Maker Workshop
3. Select Board: Click on board selector → Arduino Mega 2560
4. Select Port: Click on port selector → (your serial port)
5. Build/Compile: Click Compile button (or Cmd+B)
6. Upload: Click Upload button

Alternatively, use Command Palette (Cmd+Shift+P):
- `Arduino: Verify/Compile`
- `Arduino: Upload`

### Using Arduino IDE

1. Open `Display-Ex.ino` in Arduino IDE
2. Select Board: Tools → Board → Arduino Mega 2560
3. Select Port: Tools → Port → (your serial port)
4. Click Upload (Ctrl+U)

### Using PlatformIO

```bash
platformio run -t upload
```

## Configuration

Modify these defines in the sketch to customize:

```cpp
#define SERIAL_BAUD 115200      // Serial communication speed
#define UPDATE_INTERVAL 1000    // Display refresh rate (ms)
#define TFT_ROTATION 1          // 0=Portrait, 1=Landscape, etc.
```

## Display Features

### Current (Text Display)
- Simple text-based layout
- Color-coded status indicators
- Real-time throttle bar visualization

### Future (Graphical Display)
- Track schematic representation
- Animated locomotives
- Dynamic turnout graphics
- Touch controls for future interaction

## Troubleshooting

### Display Not Showing
- Verify wiring connections
- Check serial communication (use Serial Monitor to verify commands)
- Confirm correct board and port selected
- Try uploading the MCUFRIEND diagnostic sketch to identify display type

### Serial Communication Issues
- Verify baud rate matches (115200)
- Check USB cable and driver installation
- Monitor serial output for received messages

### Memory Issues
- If sketch is too large, enable optimization: Arduino IDE → Preferences → "Compiler warnings" → "More"
- Consider removing unused code sections

## Future Enhancements

- [ ] Graphical track layout
- [ ] Locomotive animation
- [ ] Touch screen calibration and controls
- [ ] Speed profile management
- [ ] Consist management display
- [ ] DCC diagnostics
- [ ] SD card logging of events

## Project Structure

```
Display-Ex/
├── Display-Ex.ino          # Main sketch
├── README.md               # This file
├── platformio.ini          # PlatformIO configuration
└── docs/
    └── PROTOCOL.md         # Detailed serial protocol documentation
```

## Communication Protocol Details

See [PROTOCOL.md](docs/PROTOCOL.md) for detailed information about the serial message format and extending the protocol.

## License

This project is part of the Model Railroad DCC Control System.

## Authors

- Initial Development: 2026
