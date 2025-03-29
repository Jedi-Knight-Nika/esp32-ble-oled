
#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController {
  public:
    LEDController(int redPin, int yellowPin, int greenPin);
    void init();
    void updateLEDs(bool connected, bool connecting);
    void setConnected();
    void setConnecting();
    void setDisconnected();
  
  private:
    int _redPin;
    int _yellowPin;
    int _greenPin;
};

#endif