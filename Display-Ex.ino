/*
 * DCC Monitor Display
 * 
 * Arduino Mega with ILI9486 Display
 * Monitors track status from DCC Command Station-EX
 * 
 * Displays:
 * - Track Power Status (ON/OFF)
 * - Turnout Status (3 turnouts)
 * - Throttle Setting for the configured locomotive
 */

#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <MCUFRIEND_kbv.h>
#include "config.h"

// Display instance
MCUFRIEND_kbv tft;

// The bundled MCUFRIEND library selects pins via USE_MEGA_16BIT_SHIELD.
// Sketch-level pin definitions and constructor arguments do not override it.

// Serial communication
#define BUFFER_SIZE 128

// Color definitions
#define BLACK       0x0000
#define WHITE       0xFFFF
#define RED         0xF800
#define GREEN       0x07E0
#define BLUE        0x001F
#define YELLOW      0xFFE0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define GRAY        0x8410
#define LIGHT_GRAY  0xC618

// Status structure
struct TrackStatus {
  bool powerOn;
  bool turnout1;
  bool turnout2;
  bool turnout3;
  int throttle;
  unsigned long lastUpdate;
};

TrackStatus status = {false, false, false, false, 0, 0};

// Serial buffer
char serialBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Timing
unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500); // Give serial time to initialize
  
  Serial.println("\n\n=== DCC Monitor Display Startup ===");
  Serial.println("Initializing display...");
  Serial.println("Display bus: Mega 16-bit parallel shield (bundled library)");
  
  // Initialize display
  tft.reset();
  delay(100);
  
  uint16_t identifier = tft.readID();
  Serial.print("Display ID: 0x");
  Serial.println(identifier, HEX);
  
  if (identifier == 0x9486) {
    Serial.println("ILI9486 display found - correct!");
  } else if (identifier == 0x9488) {
    Serial.println("ILI9488 display found");
  } else if (identifier == 0xD3D3 || identifier == 0x0000 || identifier == 0xFFFF) {
    Serial.println("WARNING: Display ID unreadable; controller is not verified.");
    Serial.println("Trying ILI9486 initialization for this shield.");
    identifier = 0x9486; // Force ILI9486 protocol
  } else {
    Serial.println("WARNING: Unexpected display ID!");
  }
  
  tft.begin(identifier);
  tft.setRotation(1); // Landscape mode
  testDisplay();
  
  // Clear screen and show startup message
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 50);
  tft.println("DCC Monitor");
  tft.setCursor(20, 90);
  tft.println("Starting up...");
  tft.setCursor(20, 140);
  tft.println("DCC Monitor ready");
  
  delay(2000); // Show startup message for 2 seconds
  
  // Initial display setup
  drawLayout();
  
  Serial.println("Display commands sent; confirm output on the screen.");
  Serial.println("Waiting for status messages...");
  Serial.println("Send commands like: POWER:ON");
}

void loop() {
  // Read serial data
  while (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n') {
      serialBuffer[bufferIndex] = '\0';
      processCommand(serialBuffer);
      bufferIndex = 0;
    } else if (c != '\r' && bufferIndex < BUFFER_SIZE - 1) {
      serialBuffer[bufferIndex++] = c;
    }
  }
  
  // Update display periodically
  if (millis() - lastDisplayUpdate >= UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
}

/*
 * Process incoming serial commands from DCC Command Station
 * Expected format:
 *   POWER:ON/OFF
 *   TURNOUT:<configured ID>:THROWN/OPEN/CLOSED
 *   TURNOUT:1/2/3:THROWN/OPEN/CLOSED (legacy aliases)
 *   THROTTLE:<configured loco ID>:value (0-255)
 */
bool parseCommandId(const char *text, long &id, const char *&value) {
  if (*text < '0' || *text > '9') return false;
  char *end;
  id = strtol(text, &end, 10);
  if (*end != ':' || id < 1 || id > 32767) return false;
  value = end + 1;
  return true;
}

void processCommand(const char* command) {
  if (strncmp(command, "POWER:", 6) == 0) {
    status.powerOn = strcmp(&command[6], "ON") == 0;
    Serial.print("Power: ");
    Serial.println(status.powerOn ? "ON" : "OFF");
  }
  else if (strncmp(command, "TURNOUT:", 8) == 0) {
    bool *turnouts[] = {&status.turnout1, &status.turnout2, &status.turnout3};
    const char *state;
    long id;
    int index = -1;
    if (parseCommandId(command + 8, id, state)) {
      for (int i = 0; i < 3; i++) {
        if (id == TURNOUT_IDS[i]) {
          index = i;
          break;
        }
      }
      if (index < 0 && id >= 1 && id <= 3) index = id - 1;
    }
    if (index >= 0) {
      if (strcmp(state, "THROWN") == 0 || strcmp(state, "OPEN") == 0) {
        *turnouts[index] = true;
      } else if (strcmp(state, "CLOSED") == 0) {
        *turnouts[index] = false;
      } else {
        Serial.print("Invalid turnout state: ");
        Serial.println(state);
        return;
      }
      Serial.print("Turnout ");
      Serial.print(TURNOUT_IDS[index]);
      Serial.println(*turnouts[index] ? ": OPEN" : ": CLOSED");
    } else {
      Serial.print("Unknown command: ");
      Serial.println(command);
      return;
    }
  }
  else if (strncmp(command, "THROTTLE:", 9) == 0) {
    long id;
    const char *value;
    if (!parseCommandId(command + 9, id, value) || id != LOCO_ID) {
      Serial.print("Unknown command: ");
      Serial.println(command);
      return;
    }
    status.throttle = atoi(value);
    Serial.print("Throttle ");
    Serial.print(LOCO_ID);
    Serial.print(": ");
    Serial.println(status.throttle);
  }
  else {
    Serial.print("Unknown command: ");
    Serial.println(command);
  }
  
  status.lastUpdate = millis();
}

/*
 * Draw the initial layout on the display
 */
void drawLayout() {
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor(28, 36);
  tft.print("DCC-EX Monitor");
  int16_t x1, y1;
  uint16_t width, height;
  tft.getTextBounds(LAYOUT_NAME, 0, 36, &x1, &y1, &width, &height);
  int nameX = 452 - x1 - width;
  if (nameX < 180) {
    Serial.println("WARNING: Layout name is too long for the header.");
    nameX = 180;
  }
  tft.setTextWrap(false);
  tft.setCursor(nameX, 36);
  tft.print(LAYOUT_NAME);
  tft.setTextWrap(true);
  tft.drawFastHLine(20, 47, 440, WHITE);
  tft.drawFastHLine(20, 187, 440, WHITE);
  tft.setCursor(29, 220);
  tft.print("LOCO: ");
  tft.print(LOCO_ID);
  tft.setCursor(28, 256);
  tft.print("THROTTLE");
  tft.drawRect(145, 240, 300, 20, GREEN);
  updateDisplay();
}

/*
 * Update the display with current status
 */
void updateDisplay() {
  static bool firstDraw = true;
  static bool previousPower;
  static bool previousTurnouts[3];
  static int previousThrottle;
  bool turnouts[] = {status.turnout1, status.turnout2, status.turnout3};
  int throttle = constrain(status.throttle, 0, 255);

  if (firstDraw || status.powerOn != previousPower) {
    tft.fillRect(28, 60, 200, 19, BLACK);
    tft.setTextColor(status.powerOn ? GREEN : tft.color565(255, 137, 137));
    tft.setCursor(28, 75);
    tft.print(status.powerOn ? "TRACK POWER ON" : "TRACK POWER OFF");
    previousPower = status.powerOn;
  }

  for (int i = 0; i < 3; i++) {
    if (firstDraw || turnouts[i] != previousTurnouts[i]) {
      int y = 99 + i * 28;
      tft.fillRect(28, y, 230, 19, BLACK);
      tft.setTextColor(WHITE);
      tft.setCursor(i == 2 ? 29 : 28, y + 15);
      tft.print("TURNOUT ");
      tft.print(TURNOUT_IDS[i]);
      tft.print(turnouts[i] ? ": OPEN" : ": CLOSED");
      previousTurnouts[i] = turnouts[i];
    }
  }

  if (firstDraw || throttle != previousThrottle) {
    int filledWidth = (uint32_t)298 * throttle / 255;
    tft.fillRect(146, 241, 298, 18, BLACK);
    if (filledWidth > 0) {
      tft.fillRect(146, 241, filledWidth, 18, tft.color565(217, 217, 217));
    }
    previousThrottle = throttle;
  }
  firstDraw = false;
}

/*
 * Display test - shows color blocks and text to verify display is working
 */
void testDisplay() {
  Serial.println("Running display test...");
  
  // Test 1: Fill with colors
  tft.fillScreen(RED);
  delay(500);
  tft.fillScreen(GREEN);
  delay(500);
  tft.fillScreen(BLUE);
  delay(500);
  tft.fillScreen(BLACK);
  delay(500);
  
  // Test 2: Draw rectangles
  tft.drawRect(10, 10, 100, 100, WHITE);
  tft.fillRect(130, 10, 100, 100, YELLOW);
  delay(1000);
  
  // Test 3: Text test
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("DISPLAY TEST");
  tft.setCursor(20, 60);
  tft.println("If you see this");
  tft.setCursor(20, 90);
  tft.println("the display works!");
  
  Serial.println("Display test commands sent. Continuing to DCC Monitor...");
}
