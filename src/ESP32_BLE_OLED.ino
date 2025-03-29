#include "BLEManager.h"
#include "DisplayManager.h"
#include "LEDController.h"

#define SDA_PIN 32
#define SCL_PIN 33
#define RED_LED_PIN 27
#define YELLOW_LED_PIN 26
#define GREEN_LED_PIN 25
#define DEVICE_NAME "ESP32_OLED_Status"

BLEManager bleManager(DEVICE_NAME);
DisplayManager displayManager(SDA_PIN, SCL_PIN);
LEDController ledController(RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN);

void onMessageReceived(String message)
{
    displayManager.displayText(message);
}

void onConnectionStateChanged(bool connected, bool connecting)
{
    ledController.updateLEDs(connected, connecting);

    if (connected)
    {
        displayManager.displayText("Connected!");
    }
    else if (connecting)
    {
        displayManager.displayText("Connecting...");
    }
    else
    {
        displayManager.displayText("Disconnected");
    }
}

void setup()
{
    Serial.begin(115200);

    ledController.init();
    ledController.setDisconnected();

    displayManager.init();
    displayManager.displayText("Starting BLE...");

    bleManager.init();
    bleManager.setMessageCallback(onMessageReceived);
    bleManager.setConnectionCallback(onConnectionStateChanged);

    Serial.println("BLE OLED Display with Status LEDs Ready!");
    Serial.println("Waiting for client connection...");

    displayManager.displayText("Ready for connection");
}

void loop()
{
    bleManager.update();
    
    delay(100);
}