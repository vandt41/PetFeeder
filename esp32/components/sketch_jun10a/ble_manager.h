#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "preferences_manager.h"
void BLE_Init();
void BLE_Stop();

bool BLE_HasCredentials();
void BLE_ClearCredentialFlag();
bool BLE_GetCredentials(String &ssid, String &password);
bool BLE_IsRunning();
#endif