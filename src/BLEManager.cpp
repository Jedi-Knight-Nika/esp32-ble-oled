#include "BLEManager.h"

class BLEManager::ServerCallbacks: public BLEServerCallbacks {
  private:
    BLEManager* _manager;
    
  public:
    ServerCallbacks(BLEManager* manager) : _manager(manager) {}
    
    void onConnect(BLEServer* pServer) {
      _manager->_deviceConnected = true;
      _manager->_connecting = false;
      Serial.println("Device connected");
      
      if (_manager->_connectionCallback) {
        _manager->_connectionCallback(true, false);
      }
    }

    void onDisconnect(BLEServer* pServer) {
      _manager->_deviceConnected = false;
      Serial.println("Device disconnected");
      
      if (_manager->_connectionCallback) {
        _manager->_connectionCallback(false, false);
      }
    }
};

class BLEManager::CharacteristicCallbacks: public BLECharacteristicCallbacks {
  private:
    BLEManager* _manager;
    
  public:
    CharacteristicCallbacks(BLEManager* manager) : _manager(manager) {}
    
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string stdValue = pCharacteristic->getValue();
      String value = String(stdValue.c_str());
            
      if (value.length() > 0) {
        Serial.print("Received: ");
        Serial.println(value);
        
        if (_manager->_messageCallback) {
          _manager->_messageCallback(value);
        }
      }
    }
};

BLEManager::BLEManager(const char* deviceName) {
  _deviceName = deviceName;
}

void BLEManager::init() {
  BLEDevice::init(_deviceName);
  
  _pServer = BLEDevice::createServer();
  _pServer->setCallbacks(new ServerCallbacks(this));
  
  BLEService *pService = _pServer->createService(SERVICE_UUID);
  
  _pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  
  _pCharacteristic->addDescriptor(new BLE2902());
  
  _pCharacteristic->setCallbacks(new CharacteristicCallbacks(this));
  
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  _connecting = true;
  _connectingStartTime = millis();
  
  if (_connectionCallback) {
    _connectionCallback(false, true);
  }
}

void BLEManager::update() {
  if (_connecting && (millis() - _connectingStartTime > _connectingTimeout)) {
    _connecting = false;
    
    if (_connectionCallback) {
      _connectionCallback(false, false);
    }
  }
  
  if (!_deviceConnected && _oldDeviceConnected) {
    delay(500);
    _pServer->startAdvertising(); 
    Serial.println("Started advertising again");
    
    _connecting = true;
    _connectingStartTime = millis();
    
    if (_connectionCallback) {
      _connectionCallback(false, true);
    }
    
    _oldDeviceConnected = _deviceConnected;
  }
  
  if (_deviceConnected && !_oldDeviceConnected) {
    _oldDeviceConnected = _deviceConnected;
  }
}

bool BLEManager::isConnected() {
  return _deviceConnected;
}

bool BLEManager::isConnecting() {
  return _connecting;
}

void BLEManager::setMessageCallback(void (*callback)(String)) {
  _messageCallback = callback;
}

void BLEManager::setConnectionCallback(void (*callback)(bool, bool)) {
  _connectionCallback = callback;
}