# Hardware Setup Guide

## Arduino Mega 2560 Configuration

### Pin Assignments for ILI9486 Display

The installed display is a HiLetgo 3.5-inch 480x320, 36-pin Mega2560
shield ([product listing](https://www.amazon.com/dp/B073R7Q8FF)), advertised
with an ILI9486/ILI9488 controller and a **16-bit parallel bus**, not SPI.
Plug the shield directly into the Mega with power disconnected.

The bundled MCUFRIEND_kbv library uses `USE_SPECIAL` in
`libraries/MCUFRIEND_kbv/utility/mcufriend_shield.h` and
`USE_MEGA_16BIT_SHIELD` in `utility/mcufriend_special.h`. This selects:

```
Arduino Pin    LCD Signal
-----------    ----------
37..30         D0..D7 (in that order)
22..29         D8..D15 (in that order)
43             RD
39             WR
38             RS/DC
40             CS
41             RESET
```

These are the library's Mega 16-bit mapping, not configurable sketch pins.
Defining `TFT_CS`, etc., or passing constructor arguments does not change
MCUFRIEND's wiring. Verify the shield revision/pinout if it still stays white.
Do not use the previous SPI jumper-wiring instructions.

### Display Module Specifications

- **Resolution**: 480 × 320 pixels
- **Color Depth**: 16-bit (65,536 colors)
- **Controller**: ILI9486 or ILI9488 (listing; read ID when supported)
- **Interface**: 16-bit parallel
- **Voltage**: 3.3V logic, 5V power capable
- **Backlight**: LED (adjustable brightness via PWM if needed)

### Serial Connection for Command Station-EX

Connect a USB-to-Serial adapter or direct connection:

```
Arduino Mega          Adapter/Command Station
-----------          -----------------------
TX1 (pin 18) ------> RX
RX1 (pin 19) ------> TX
GND ----------------> GND
```

The sketch reads both Serial (USB/pins 0-1) and Serial1 (pins 18-19).
Using Serial1 leaves the USB serial port available for uploads and diagnostics.

## Power Supply Considerations

### Arduino Mega Power
- Supply 7-12V via barrel connector or USB
- Internal 5V regulator provides power for logic and ILI9486 logic levels
- Current draw: ~100-200mA typical

### Display Power
- Logic: 3.3V (ILI9486 chip)
- VCC: 5V (power supply pin)
- Backlight: 5V @ 100-200mA typical
- **Total System Current**: ~300-500mA

### Recommended Power Supply
- 12V @ 1A minimum for Arduino
- Shared ground between Arduino and display module

## Troubleshooting Connection

### Display Not Responding

1. **Verify power**:
   - Check 5V to display VCC pins
   - Check GND connections (should be common)
   - Use multimeter to verify voltage at display

2. **Check the parallel bus configuration**:
   - Confirm the compile output selects `USE_MEGA_16BIT_SHIELD` from the
     bundled library, not the default UNO shield mapping.
   - Serial Monitor (115200 baud) prints the raw controller ID.
   - `0xD3D3` can occur on write-only shields; it does not identify a controller.
     The sketch explicitly warns before trying ILI9486 as a fallback.

3. **Reset display**:
   - Briefly (< 100ms) pull RST low, then high
   - This should be automatic in setup()

4. **Verify MCUFRIEND library**:
   - The sketch now runs its existing color/text test before drawing the layout.
   - For deeper tests, use `graphictest_kbv` in its own sketch folder.
   - Arduino compiles all `.ino` files in a sketch folder, even closed editor tabs.
     A separate diagnostic needs its own folder and matching primary filename.

### Serial Communication Issues

1. **Check baud rate**: Must be 115200
2. **Monitor input**:
   - Open Serial Monitor in Arduino IDE
   - Send: `POWER:ON` (and press Send)
   - Should see: `Power: ON`

3. **Check connections**:
   - TX1/RX1 not reversed
   - Cross-over if needed (some adapters swap TX/RX)

## Alternative Display Modules

If you have a different ILI9486 module variant:

1. Run the MCUFRIEND `graphictest_kbv` example in its own sketch folder
2. Note the display ID it reports
3. Update the library or pin configuration as needed
4. Other shields may use different 8-bit or 16-bit parallel mappings.
   Select the matching library driver only after verifying the hardware pinout.

## Testing After Assembly

1. Load `Display-Ex.ino` into Arduino IDE
2. Verify board selection: Arduino Mega 2560
3. Select correct COM port
4. Click Upload (Ctrl+U)
5. Open Serial Monitor (115200 baud)
6. You should see:
   ```
   Display bus: Mega 16-bit parallel shield (bundled library)
   ...
   Display commands sent; confirm output on the screen.
   Waiting for status messages...
   ```
7. Verify red, green and blue screens, then test text and the DCC layout.
   Serial messages only confirm commands were sent, not that the LCD responded.
8. Send test commands:
   ```
   POWER:ON
   TURNOUT:1:THROWN
   THROTTLE:3:128
   ```
9. Display should update with new values

## Performance Optimization

If display updates are slow:

1. Keep the Mega 16-bit parallel driver enabled. SPI clock settings do not
   affect this display.

2. Reduce display update frequency:
   ```cpp
   #define UPDATE_INTERVAL 2000  // 2 seconds instead of 1
   ```

3. Use partial screen updates instead of full redraw (future enhancement)

## Safety Notes

- **ESD**: Ground yourself before touching components
- **Power**: Double-check polarity before supplying power
- **Heat**: Display module can dissipate heat; ensure ventilation
- **Current**: Don't exceed 500mA on Arduino 5V rail; add external power if needed
