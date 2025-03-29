#include "DisplayManager.h"

DisplayManager::DisplayManager(int sdaPin, int sclPin) 
  : _sdaPin(sdaPin), 
    _sclPin(sclPin),
    _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

void DisplayManager::init() {
  Wire.begin(_sdaPin, _sclPin);
  
  if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  _display.println("Initializing...");
  _display.display();
}

void DisplayManager::displayText(String text) {
  _currentText = text;
  
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  
  _display.print("Status: ");
  _display.print(text == "Connected!" ? "Connected" : 
                (text == "Connecting..." ? "Connecting..." : 
                (text == "Disconnected" ? "Disconnected" : "Ready")));
  
  _display.drawLine(0, 10, _display.width(), 10, SSD1306_WHITE);
  
  _display.setCursor(0, 16);
  _display.setTextSize(1);
  
  int16_t x1, y1;
  uint16_t w, h;
  int16_t xpos = 0;
  int16_t ypos = 16;
  
  for (unsigned int i = 0; i < _currentText.length(); i++) {
    char c = _currentText.charAt(i);
    
    _display.getTextBounds(String(c), xpos, ypos, &x1, &y1, &w, &h);
    
    if (xpos + w > SCREEN_WIDTH) {
      xpos = 0;
      ypos += h + 2; 
    }
    
    if (ypos > SCREEN_HEIGHT) break;
    
    _display.setCursor(xpos, ypos);
    _display.print(c);
    
    xpos += w;
  }
  
  _display.display();
}