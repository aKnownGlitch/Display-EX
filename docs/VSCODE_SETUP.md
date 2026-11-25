# VS Code Arduino Development Setup

This guide covers setting up and using this Arduino project with **VS Code and the Arduino Maker Workshop extension**.

## Prerequisites

- VS Code installed
- Arduino IDE or Arduino CLI installed (for board packages and tools)
- Arduino Maker Workshop extension installed in VS Code

## Installation & Setup

### 1. Install Arduino Maker Workshop Extension

1. Open VS Code
2. Click Extensions icon (or Cmd+Shift+X)
3. Search for "Arduino Maker Workshop"
4. Click Install (by Microsoft)
5. Reload VS Code when prompted

The extension will automatically detect your Arduino installation or guide you through setup.

### 2. First Time Setup

When you first use the extension:
- It will prompt you to select the Arduino installation path
- If using Arduino IDE, it will find it automatically
- If using Arduino CLI, it will guide you to configure it
- Let it install any required tools

### 3. Open This Project

1. File → Open Folder
2. Navigate to: `/Volumes/Extreme Pro/src/arduino/Display-Ex`
3. Click Open
4. VS Code recognizes it as an Arduino project
5. The Arduino icon will appear in the Activity Bar (left sidebar)

## Basic Workflow

### Accessing Arduino Tools

Click the **Arduino icon** in the left sidebar to access:
- Board Manager - Select Arduino Mega 2560
- Port Selector - Choose USB serial port
- Library Manager - Install/manage libraries
- Compile - Build the sketch
- Upload - Upload to board
- Serial Monitor - Debug output

Alternatively, use **Command Palette** (Cmd+Shift+P) and type:
- `Arduino: Select Board`
- `Arduino: Select Serial Port`
- `Arduino: Verify/Compile`
- `Arduino: Upload`
- `Arduino: Open Serial Monitor`

### Step-by-Step Compilation and Upload

1. **Select Board**
   - Click Arduino icon → Board Manager
   - Search and select "Arduino Mega 2560"

2. **Select Port**
   - Click Arduino icon → Port Selector
   - Choose your USB port (e.g., `/dev/cu.usbserial-14401`)

3. **Install Libraries** (first time only)
   - Click Arduino icon → Library Manager
   - Search: "Adafruit GFX Library" → Install
   - Search: "MCUFRIEND_kbv" → Install
   - Wait for installation to complete

4. **Compile the Sketch**
   - Click Arduino icon → Compile
   - Or press Cmd+B
   - Or use Command Palette → `Arduino: Verify/Compile`
   - Wait for "Build complete" message

5. **Upload to Arduino**
   - Click Arduino icon → Upload
   - Or use Command Palette → `Arduino: Upload`
   - Wait for "Upload complete"
   - Display should initialize and show status

6. **Monitor Serial Output**
   - Click Arduino icon → Serial Monitor
   - Or use Command Palette → `Arduino: Open Serial Monitor`
   - Set baud rate to **115200** (top of monitor)
   - You should see: `DCC Monitor Display initialized`
   - Try sending: `POWER:ON` (type and press Enter)
   - Display should update

## Project Structure in VS Code

```
Display-Ex/
├── 📄 Display-Ex.ino          ← Main sketch (open to edit)
├── 📄 Display-Ex-Test.ino     ← Test sketch
├── 📄 README.md               ← Documentation
├── 📄 QUICKSTART.md           ← Quick start guide
├── 📄 PROJECT_SUMMARY.txt     ← Reference guide
├── 📂 docs/
│   ├── PROTOCOL.md            ← Serial protocol
│   ├── HARDWARE_SETUP.md      ← Wiring diagram
│   └── VSCODE_SETUP.md        ← This file
├── platformio.ini             ← Build config
└── .gitignore                 ← Git rules
```

## Editing and IntelliSense

### Code Completion
The Arduino Maker Workshop provides IntelliSense for:
- Arduino built-in functions (Serial, digitalWrite, etc.)
- Display library functions (tft.fillScreen, etc.)
- Your custom functions

Type a function and press Ctrl+Space for suggestions.

### Quick Help
- Hover over a function name to see its documentation
- Use Cmd+Click to jump to function definition
- Use Ctrl+Shift+O to see document symbols (outline view)

## Common Tasks

### Compile Only (No Upload)
- Click Arduino icon → Compile
- Or press Cmd+B
- Or Command Palette → `Arduino: Verify/Compile`
- Useful for checking syntax errors before upload

### Upload to Board
- Click Arduino icon → Upload
- Or Command Palette → `Arduino: Upload`
- Compilation happens automatically first

### Open Serial Monitor
- Click Arduino icon → Serial Monitor
- Monitor window opens at bottom
- Type messages and press Enter
- Messages appear in real-time

### Change Board
- Click Arduino icon → Board Manager
- Search for board
- Click to select
- Status bar updates with selected board

### Change Serial Port
- Click Arduino icon → Port Selector
- List shows all available ports
- Click to select your Arduino's port
- Status bar updates with selected port

### Install Libraries
- Click Arduino icon → Library Manager
- Search by name (e.g., "Adafruit")
- Click Install on desired package
- Wait for download and installation
- Dependencies install automatically

## Status Bar Information

The bottom status bar shows:
- **Board Name** (e.g., "Arduino Mega 2560")
- **Serial Port** (e.g., "/dev/cu.usbserial")
- **Status Messages** during compilation/upload

Click any item to change it quickly.

## Troubleshooting

### "Arduino extension not found" or "Arduino tools not installed"
- Arduino IDE must be installed on your system
- Or install Arduino CLI separately
- Maker Workshop will detect and configure automatically
- Restart VS Code after installing Arduino IDE

### "Board 'Arduino Mega 2560' not found"
- Arduino Mega 2560 board package not installed
- In VS Code: Arduino icon → Board Manager
- Search "Arduino Mega"
- Install "Arduino AVR Boards" package
- Reload VS Code

### "Port not available"
- Arduino not connected via USB
- Check USB cable is properly connected
- Try different USB port on computer
- Check Device Manager / System Profiler for Arduino device
- In Arduino IDE: select port manually to verify

### Compilation fails with "library not found"
- Library not installed
- Arduino icon → Library Manager
- Search for missing library name
- Install from list
- Compilation uses correct library search paths

### Upload fails with "port already in use"
- Close Serial Monitor first (open only one at a time)
- Or try different port if available
- Restart Arduino Maker Workshop: Command Palette → `Arduino: Restart`

### Display shows garbage or nothing
- Verify hardware wiring per HARDWARE_SETUP.md
- Run Display-Ex-Test.ino to test display
- Try different rotation values: `tft.setRotation(0,1,2,3)`
- Check power connections

## Tips & Tricks

### Quick Keyboard Shortcuts
- `Cmd+B` - Compile
- `Cmd+Shift+P` - Command Palette
- `Cmd+Shift+M` - Toggle Problems panel
- `Cmd+Shift+D` - Toggle Debug panel
- `Cmd+Shift+G` - Git commands

### Smart Compilation
- Always compile before upload
- Compilation shows syntax errors clearly
- Fix errors in editor and recompile
- Only upload when compilation succeeds

### Serial Communication Debugging
- Open Serial Monitor while running
- Send test messages to verify sketch is working
- Baud rate MUST match (115200 for this project)
- Use Serial.println() in code to debug
- Watch messages appear in real-time in Serial Monitor

### File Organization
- Keep related sketches in same folder
- Create subfolders for libraries if needed
- Comment your code extensively
- Use meaningful variable names

### Version Control
- Git integration built into VS Code
- Use Source Control panel (Cmd+Shift+G) to commit
- Repo already initialized with 3 commits
- `git log --oneline` to see history

## Advanced: Custom Tasks

You can create custom tasks for repetitive operations. Create `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Arduino: Compile & Upload",
      "type": "shell",
      "command": "code",
      "args": ["--command", "arduino.upload"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

Then run with Cmd+Shift+B.

## Multi-File Sketches

This project is a single-file sketch, but Maker Workshop supports:
- Multiple `.ino` files in same folder
- Each file is concatenated during compilation
- Useful for larger projects with multiple components
- Main file (Display-Ex.ino) is compiled first

## Resources

- **Arduino Maker Workshop Docs** - In VS Code Help
- **Arduino Official** - https://www.arduino.cc/
- **This Project Docs** - See README.md, QUICKSTART.md, PROJECT_SUMMARY.txt
- **Troubleshooting** - See HARDWARE_SETUP.md

## Next Steps

1. ✅ Set up Maker Workshop extension
2. ✅ Open this project folder
3. ✅ Select board and port
4. ✅ Install required libraries
5. ✅ Compile the sketch
6. ✅ Upload to Arduino Mega
7. ✅ Monitor serial output
8. ✅ Test with display messages

---

**Happy coding!**

For project-specific help, see:
- [QUICKSTART.md](../QUICKSTART.md) - Quick start guide
- [README.md](../README.md) - Full documentation
- [HARDWARE_SETUP.md](./HARDWARE_SETUP.md) - Wiring diagram
- [PROTOCOL.md](./PROTOCOL.md) - Serial protocol
