#include "LEDController.h"

LEDController::LEDController(int redPin, int yellowPin, int greenPin) 
  : _redPin(redPin), 
    _yellowPin(yellowPin), 
    _greenPin(greenPin) {
}

void LEDController::init() {
  pinMode(_redPin, OUTPUT);
  pinMode(_yellowPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  
  digitalWrite(_redPin, LOW);
  digitalWrite(_yellowPin, LOW);
  digitalWrite(_greenPin, LOW);
}

void LEDController::updateLEDs(bool connected, bool connecting) {
  if (connected) {
    setConnected();
  } else if (connecting) {
    setConnecting();
  } else {
    setDisconnected();
  }
}

void LEDController::setConnected() {
  digitalWrite(_greenPin, HIGH);
  digitalWrite(_yellowPin, LOW);
  digitalWrite(_redPin, LOW);
}

void LEDController::setConnecting() {
  digitalWrite(_greenPin, LOW);
  digitalWrite(_yellowPin, HIGH);
  digitalWrite(_redPin, LOW);
}

void LEDController::setDisconnected() {
  digitalWrite(_greenPin, LOW);
  digitalWrite(_yellowPin, LOW);
  digitalWrite(_redPin, HIGH);
}