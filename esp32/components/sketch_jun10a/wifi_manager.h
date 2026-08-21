#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "preferences_manager.h"

#define WIFI_CONNECT_TIMEOUT_MS 15000

bool WiFi_Connect(const String& ssid, const String& password);
//If saved credentials connect succesfully, false otherwise(wrong or no ssid/password)
bool WiFi_ConnectSaved();
bool WiFi_IsConnected();

#endif