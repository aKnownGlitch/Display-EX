/*
 * ILI9486 Display Test Sketch
 * 
 * This is a diagnostic sketch to verify your ILI9486 display is properly
 * connected and functional. Use this for troubleshooting before running
 * the main Display-Ex sketch.
 * 
 * To use:
 * 1. Save this as a new sketch (File → New)
 * 2. Upload to Arduino Mega
 * 3. Open Serial Monitor (115200 baud)
 * 4. Watch the display and serial output
 * 
 * Expected behavior:
 * - Display shows colored rectangles
 * - Text appears on screen
 * - Serial monitor shows "Display found: 0x9486"
 */

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

// Color definitions
#define BLACK       0x0000
#define WHITE       0xFFFF
#define RED         0xF800
#define GREEN       0x07E0
#define BLUE        0x001F
#define YELLOW      0xFFE0
#define CYAN        0x07FF
#define MAGENTA     0xF81F

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\nILI9486 Display Test");
  Serial.println("====================\n");
  
  Serial.print("Attempting to initialize display...");
  tft.reset();
  uint16_t identifier = tft.readID();
  
  Serial.print("Display ID: 0x");
  Serial.println(identifier, HEX);
  
  if (identifier == 0x9486) {
    Serial.println("✓ ILI9486 found - Display is correctly identified!");
  } else if (identifier == 0x00 || identifier == 0xFF) {
    Serial.println("✗ No display detected - check wiring!");
    return;
  } else {
    Serial.print("⚠ Unknown display type. Attempting initialization anyway...");
  }
  
  tft.begin(identifier);
  tft.setRotation(1);  // Landscape
  
  Serial.println("\nStarting display tests...\n");
  
  // Test 1: Fill screen with colors
  Serial.println("Test 1: Color fill");
  colorFillTest();
  delay(2000);
  
  // Test 2: Geometric shapes
  Serial.println("Test 2: Shapes");
  shapeTest();
  delay(2000);
  
  // Test 3: Text
  Serial.println("Test 3: Text display");
  textTest();
  delay(2000);
  
  // Final test: Animation
  Serial.println("Test 4: Animation (rotating colors)\n");
  animationTest();
}

void loop() {
  // Animation continues
  animationTest();
  delay(500);
}

void colorFillTest() {
  int colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE};
  char* names[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", "CYAN", "WHITE"};
  
  for (int i = 0; i < 7; i++) {
    tft.fillScreen(colors[i]);
    Serial.print("  ");
    Serial.println(names[i]);
    delay(500);
  }
  
  tft.fillScreen(BLACK);
}

void shapeTest() {
  // Draw rectangles
  Serial.println("  Drawing rectangles...");
  tft.fillScreen(BLACK);
  tft.drawRect(10, 10, 100, 100, WHITE);
  tft.fillRect(130, 10, 100, 100, RED);
  tft.drawRect(250, 10, 100, 100, GREEN);
  
  // Draw circles
  Serial.println("  Drawing circles...");
  tft.drawCircle(60, 180, 40, BLUE);
  tft.fillCircle(180, 180, 40, YELLOW);
  tft.drawCircle(300, 180, 40, MAGENTA);
  
  // Draw lines
  Serial.println("  Drawing lines...");
  tft.drawLine(10, 280, 470, 280, CYAN);
  tft.drawLine(10, 290, 470, 290, WHITE);
}

void textTest() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  
  // Test different text sizes
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  tft.println("Text Size 1 - Normal");
  Serial.println("  Text Size 1");
  
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.println("Text Size 2");
  Serial.println("  Text Size 2");
  
  tft.setTextSize(3);
  tft.setCursor(10, 100);
  tft.println("Text Size 3");
  Serial.println("  Text Size 3");
  
  // Colored text
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.setCursor(10, 160);
  tft.println("Red Text");
  
  tft.setTextColor(GREEN);
  tft.setCursor(10, 190);
  tft.println("Green Text");
  
  tft.setTextColor(BLUE);
  tft.setCursor(10, 220);
  tft.println("Blue Text");
  
  tft.setTextColor(YELLOW);
  tft.setCursor(10, 250);
  tft.println("Yellow Text");
  
  Serial.println("  All text sizes displayed");
}

void animationTest() {
  static int state = 0;
  int colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN};
  char* names[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", "CYAN"};
  
  tft.fillScreen(BLACK);
  
  // Fill quadrants with colors
  int quads[] = {colors[state], colors[(state+1)%6], 
                 colors[(state+2)%6], colors[(state+3)%6]};
  
  tft.fillRect(0, 0, 240, 160, quads[0]);
  tft.fillRect(240, 0, 240, 160, quads[1]);
  tft.fillRect(0, 160, 240, 160, quads[2]);
  tft.fillRect(240, 160, 240, 160, quads[3]);
  
  // Display current state
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(150, 140);
  tft.println("Test OK!");
  
  state = (state + 1) % 6;
}
