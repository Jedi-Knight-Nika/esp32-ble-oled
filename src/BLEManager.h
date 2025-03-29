#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

class BLEManager {
  public:
    BLEManager(const char* deviceName);
    void init();
    void update();
    bool isConnected();
    bool isConnecting();
    void setMessageCallback(void (*callback)(String));
    void setConnectionCallback(void (*callback)(bool, bool));
  
  private:
    const char* _deviceName;
    BLEServer* _pServer = NULL;
    BLECharacteristic* _pCharacteristic = NULL;
    bool _deviceConnected = false;
    bool _oldDeviceConnected = false;
    bool _connecting = false;
    unsigned long _connectingStartTime = 0;
    const long _connectingTimeout = 5000; 
    
    void (*_messageCallback)(String) = NULL;
    void (*_connectionCallback)(bool, bool) = NULL;
    
    const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
    
    class ServerCallbacks;
    class CharacteristicCallbacks;
};

#endif