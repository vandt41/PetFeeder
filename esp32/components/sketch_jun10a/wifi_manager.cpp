#include "wifi_manager.h"

bool WiFi_Connect(const String& ssid,
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

//If saved credentials fail to connect(wrong or no ssid/password) -> return false
bool WiFi_ConnectSaved()
{
    WifiCredentials_t creds;

    if(!LoadCredentials(creds))
    {
        return false;
    }

    return WiFi_Connect(
        creds.ssid,
        creds.password);
}

bool WiFi_IsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}
