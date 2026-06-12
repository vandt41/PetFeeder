#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "preferences_manager.h"

#define WIFI_CONNECT_TIMEOUT_MS 15000

static bool WiFi_Connect(const String& ssid,
                         const String& password)
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid.c_str(),
               password.c_str());

    uint32_t startTime = millis();

    Serial.print("Connecting to ");
    Serial.println(ssid);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");

        if(millis() - startTime >
           WIFI_CONNECT_TIMEOUT_MS)
        {
            Serial.println();
            Serial.println("Connection timeout");

            return false;
        }
    }

    Serial.println();
    Serial.println("WiFi Connected");

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    return true;
}

static bool WiFi_ConnectSaved()
{
    WifiCredentials creds;

    if(!LoadCredentials(creds))
    {
        return false;
    }

    return WiFi_Connect(
        creds.ssid,
        creds.password);
}

static bool WiFi_IsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

#endif