#include <Arduino.h>

#include "BLEDevice.h"
#include "BLE2902.h"

void setup() {
  Serial.begin(115200);

  BLEDevice::init("abcd");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService("91bad492-b950-4226-aa2b-4ede9fa42f59");
  BLECharacteristic* pCharacteristic = pService->createCharacteristic(
      BLEUUID("0d563a58-196a-48ce-ace2-dfec78acc814"),
      BLECharacteristic::PROPERTY_BROADCAST | BLECharacteristic::PROPERTY_READ  |
      BLECharacteristic::PROPERTY_NOTIFY    | BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_INDICATE
  );
  pCharacteristic->setValue("Hello World!");
  BLE2902* p2902Descriptor = new BLE2902();
  p2902Descriptor->setNotifications(true);
  pCharacteristic->addDescriptor(p2902Descriptor);

  pService->start();

  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));
  pAdvertising->start();
}

void loop() { }
