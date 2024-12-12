#include <Arduino.h>

#include "BLEDevice.h"
#include "BLE2902.h"

BLEAdvertisedDevice ad;
bool connect = false;
BLEUUID serviceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");
BLEUUID    charUUID("0d563a58-196a-48ce-ace2-dfec78acc814");
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      advertisedDevice.getScan()->stop();
      ad = advertisedDevice;
      connect = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(100);
}

void loop() {
  while (!connect) { delay(100); }
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->connect(&ad);
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    return;}
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    return;}
  Serial.println(pRemoteCharacteristic->readValue().c_str());
  pRemoteCharacteristic->writeValue("abcd");
  pClient->disconnect();
  delay(1000000);
}
