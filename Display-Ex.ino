/*
 * DCC Monitor Display
 * 
 * Arduino Mega with ILI9486 Display
 * Monitors track status from DCC Command Station-EX
 * 
 * Displays:
 * - Track Power Status (ON/OFF)
 * - Turnout Status (3 turnouts)
 * - Throttle Setting (Loco 3)
 */

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

// Display instance
MCUFRIEND_kbv tft;

// Pin definitions for Arduino Mega + ILI9486
// Using default SPI pins and control pins for MCUFRIEND
#define TFT_CS   10
#define TFT_CD   9
#define TFT_WR   8
#define TFT_RD   7
#define TFT_RESET 6

// Serial communication
#define SERIAL_BAUD 115200
#define BUFFER_SIZE 128

// Display update interval (milliseconds)
#define UPDATE_INTERVAL 1000

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
  int throttle3;
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
  
  // Initialize display
  tft.reset();
  uint16_t identifier = tft.readID();
  
  if (identifier == 0x9486) {
    Serial.println("ILI9486 display found");
  } else {
    Serial.print("Display ID: 0x");
    Serial.println(identifier, HEX);
  }
  
  tft.begin(identifier);
  tft.setRotation(1); // Landscape mode
  
  // Initial display setup
  drawLayout();
  
  Serial.println("DCC Monitor Display initialized");
  Serial.println("Waiting for status messages...");
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
 *   TURNOUT:1:THROWN/CLOSED
 *   TURNOUT:2:THROWN/CLOSED
 *   TURNOUT:3:THROWN/CLOSED
 *   THROTTLE:3:value (0-255)
 */
void processCommand(const char* command) {
  if (strncmp(command, "POWER:", 6) == 0) {
    status.powerOn = strcmp(&command[6], "ON") == 0;
    Serial.print("Power: ");
    Serial.println(status.powerOn ? "ON" : "OFF");
  }
  else if (strncmp(command, "TURNOUT:1:", 10) == 0) {
    status.turnout1 = strcmp(&command[10], "THROWN") == 0;
    Serial.print("Turnout 1: ");
    Serial.println(status.turnout1 ? "THROWN" : "CLOSED");
  }
  else if (strncmp(command, "TURNOUT:2:", 10) == 0) {
    status.turnout2 = strcmp(&command[10], "THROWN") == 0;
    Serial.print("Turnout 2: ");
    Serial.println(status.turnout2 ? "THROWN" : "CLOSED");
  }
  else if (strncmp(command, "TURNOUT:3:", 10) == 0) {
    status.turnout3 = strcmp(&command[10], "THROWN") == 0;
    Serial.print("Turnout 3: ");
    Serial.println(status.turnout3 ? "THROWN" : "CLOSED");
  }
  else if (strncmp(command, "THROTTLE:3:", 11) == 0) {
    status.throttle3 = atoi(&command[11]);
    Serial.print("Throttle 3: ");
    Serial.println(status.throttle3);
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
  
  // Title
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 20);
  tft.println("DCC Monitor");
  
  // Divider line
  tft.drawLine(0, 70, 480, 70, GRAY);
  
  // Section labels
  tft.setTextSize(2);
  
  // Power section
  tft.setTextColor(YELLOW);
  tft.setCursor(20, 90);
  tft.println("Track Power:");
  
  // Turnouts section
  tft.setCursor(20, 160);
  tft.println("Turnouts:");
  
  // Throttle section
  tft.setCursor(20, 260);
  tft.println("Loco 3 Throttle:");
  
  updateDisplay();
}

/*
 * Update the display with current status
 */
void updateDisplay() {
  uint16_t powerColor = status.powerOn ? GREEN : RED;
  
  // Update Power Status
  tft.fillRect(300, 85, 150, 50, BLACK);
  tft.setTextColor(powerColor);
  tft.setTextSize(3);
  tft.setCursor(310, 100);
  tft.println(status.powerOn ? "ON" : "OFF");
  
  // Update Turnout Status
  uint16_t y = 180;
  tft.setTextSize(2);
  
  // Turnout 1
  tft.fillRect(200, y, 200, 25, BLACK);
  tft.setTextColor(status.turnout1 ? MAGENTA : CYAN);
  tft.setCursor(210, y + 5);
  tft.print("T1: ");
  tft.println(status.turnout1 ? "THROWN" : "CLOSED");
  
  // Turnout 2
  y += 30;
  tft.fillRect(200, y, 200, 25, BLACK);
  tft.setTextColor(status.turnout2 ? MAGENTA : CYAN);
  tft.setCursor(210, y + 5);
  tft.print("T2: ");
  tft.println(status.turnout2 ? "THROWN" : "CLOSED");
  
  // Turnout 3
  y += 30;
  tft.fillRect(200, y, 200, 25, BLACK);
  tft.setTextColor(status.turnout3 ? MAGENTA : CYAN);
  tft.setCursor(210, y + 5);
  tft.print("T3: ");
  tft.println(status.turnout3 ? "THROWN" : "CLOSED");
  
  // Update Throttle Setting
  tft.fillRect(300, 275, 150, 50, BLACK);
  tft.setTextColor(LIGHT_GRAY);
  tft.setTextSize(3);
  tft.setCursor(310, 290);
  tft.print(status.throttle3);
  tft.println("/255");
  
  // Draw throttle bar
  drawThrottleBar(210, 310, 240, 30);
}

/*
 * Draw a visual representation of the throttle setting
 */
void drawThrottleBar(int x, int y, int width, int height) {
  // Background
  tft.drawRect(x, y, width, height, GRAY);
  
  // Filled portion
  int filledWidth = (width - 2) * status.throttle3 / 255;
  if (filledWidth > 0) {
    tft.fillRect(x + 1, y + 1, filledWidth, height - 2, GREEN);
  }
}
