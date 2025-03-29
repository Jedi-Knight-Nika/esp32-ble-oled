#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class DisplayManager {
  public:
    DisplayManager(int sdaPin, int sclPin);
    void init();
    void displayText(String text);
  
  private:
    int _sdaPin;
    int _sclPin;
    String _currentText = "";
    
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = -1;
    static const int SCREEN_ADDRESS = 0x3C;
    
    Adafruit_SSD1306 _display;
};

#endif 