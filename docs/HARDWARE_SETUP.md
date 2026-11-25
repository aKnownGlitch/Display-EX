# Hardware Setup Guide

## Arduino Mega 2560 Configuration

### Pin Assignments for ILI9486 Display

The sketch uses the following pin configuration (can be modified in the code):

```
Arduino Pin    Signal    ILI9486 Pin    Description
-------        ------    -----------    -----------
50             MISO      12 (SDO)       Data input from display
51             MOSI      11 (SDI)       Data output to display
52             SCK       13 (SCK)       Serial clock
10             CS        10             Chip select (active low)
9              DC/RS     9              Data/Command control
8              WR        8              Write strobe
7              RD        7              Read strobe
6              RST       25             Reset
GND            GND       1,2,20         Ground
5V             VCC       3,4            Power supply
5V             LED+      19             Backlight positive
GND            LED-      20             Backlight negative
```

### Display Module Specifications

- **Resolution**: 480 × 320 pixels
- **Color Depth**: 16-bit (65,536 colors)
- **Controller**: ILI9486
- **Interface**: 8-bit or 16-bit SPI
- **Voltage**: 3.3V logic, 5V power capable
- **Backlight**: LED (adjustable brightness via PWM if needed)

### Wiring Diagram

```
Arduino Mega          ILI9486 Module
-----------          ---------------
         50 -------> SDO (12)
         51 -------> SDI (11)
         52 -------> SCK (13)
         10 -------> CS  (10)
          9 -------> DC  (9)
          8 -------> WR  (8)
          7 -------> RD  (7)
          6 -------> RST (25)
        GND -------> GND (1,2,20)
         5V -------> VCC (3,4)
         5V -------> LED+ (19)
        GND -------> LED- (20)
```

### Serial Connection for Command Station-EX

Connect a USB-to-Serial adapter or direct connection:

```
Arduino Mega          Adapter/Command Station
-----------          -----------------------
TX1 (pin 18) ------> RX
RX1 (pin 19) ------> TX
GND ----------------> GND
```

**Note**: If using multiple serial ports on Mega, can use Serial1-Serial3 instead of Serial.

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

2. **Check SPI pins**:
   ```bash
   # In Arduino IDE Serial Monitor (115200 baud)
   # You should see: "Display ID: 0x9486" or "ILI9486 display found"
   ```

3. **Reset display**:
   - Briefly (< 100ms) pull RST low, then high
   - This should be automatic in setup()

4. **Verify MCUFRIEND library**:
   - Use example sketch: `File → Examples → MCUFRIEND_kbv → Diagnostic`
   - This will help identify display type and configuration

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

1. Run the MCUFRIEND diagnostic sketch
2. Note the display ID it reports
3. Update the library or pin configuration as needed
4. Some modules use 8-bit parallel instead of SPI (would require code changes)

## Testing After Assembly

1. Load `Display-Ex.ino` into Arduino IDE
2. Verify board selection: Arduino Mega 2560
3. Select correct COM port
4. Click Upload (Ctrl+U)
5. Open Serial Monitor (115200 baud)
6. You should see:
   ```
   ILI9486 display found
   DCC Monitor Display initialized
   Waiting for status messages...
   ```
7. Send test commands:
   ```
   POWER:ON
   TURNOUT:1:THROWN
   THROTTLE:3:128
   ```
8. Display should update with new values

## Performance Optimization

If display updates are slow:

1. Increase SPI clock speed:
   ```cpp
   // In Display-Ex.ino setup()
   SPI.setClockDivider(SPI_CLOCK_DIV2);  // Faster SPI
   ```

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
