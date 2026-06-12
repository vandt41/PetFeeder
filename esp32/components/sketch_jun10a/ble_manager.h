#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "preferences_manager.h"

#define SERVICE_UUID        "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SSID_UUID           "19b10001-e8f2-537e-4f6c-d104768a1214"
#define PASSWORD_UUID       "19b10002-e8f2-537e-4f6c-d104768a1214"

static BLEServer* pServer = NULL;

static String receivedSSID;
static String receivedPassword;

static bool credentialsReady = false;

// Server callbacks class
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Client connected!");
  };
  
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Client disconnected!");
  }
};
class SSIDCallbacks :
    public BLECharacteristicCallbacks
{
    void onWrite(
        BLECharacteristic* pChar)
    {
        receivedSSID =
            String(pChar->getValue().c_str());

        Serial.print("SSID: ");
        Serial.println(receivedSSID);

        if(receivedPassword.length())
        {
            SaveCredentials(
                receivedSSID,
                receivedPassword);

            credentialsReady = true;
        }
    }
};

class PasswordCallbacks :
    public BLECharacteristicCallbacks
{
    void onWrite(
        BLECharacteristic* pChar)
    {
        receivedPassword =
            String(pChar->getValue().c_str());

        Serial.println("Password received");

        if(receivedSSID.length())
        {
            SaveCredentials(
                receivedSSID,
                receivedPassword);

            credentialsReady = true;
        }
    }
};

static void BLE_Init()
{
    BLEDevice::init("PetFeeder");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService* service =
        pServer->createService(
            SERVICE_UUID);

    BLECharacteristic* ssidChar =
        service->createCharacteristic(
            SSID_UUID,
            BLECharacteristic::PROPERTY_WRITE);

    BLECharacteristic* passChar =
        service->createCharacteristic(
            PASSWORD_UUID,
            BLECharacteristic::PROPERTY_WRITE);

    ssidChar->setCallbacks(
        new SSIDCallbacks());

    passChar->setCallbacks(
        new PasswordCallbacks());

    service->start();
    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

static void BLE_Stop()
{
    BLEDevice::stopAdvertising();

    Serial.println("BLE Stopped");
}

static bool BLE_HasCredentials()
{
    return credentialsReady;
}

static void BLE_ClearCredentialFlag()
{
    credentialsReady = false;
}

#endif