#include "ble_manager.h"
#include "preferences_manager.h"
#include "wifi_manager.h"
// #include "ntp_server.h"
#include "stdint.h"
#include "usart_com.h"
#include <WebServer.h>
WebServer server(80);
int foodLevel;
String lastFed;
void setup()
{
    Serial.begin(115200);

    USART_Init();

    if(WiFi_ConnectSaved())
    {
        Serial.println("Connected using saved credentials");
        server.on("/status", HTTP_GET, handleStatus);
        server.on("/feed", HTTP_POST, handleFeed);

        server.begin();

        Serial.println("Web server started");
        Serial.println(WiFi.localIP());
    }
    if (WiFi_IsConnected())
    {
        Serial.println("Connected!");
    }
else
{
    Serial.println("Not connected");
}
}

void loop()
{
    server.handleClient();
    // Receive UART commands from STM32
    CommandPacket_t pkt;

    if(readCmdPacket(pkt))
    {
        switch(pkt.command)
        {
            case CMD_INIT:
                BLE_Init();
                break;

            case CMD_FEED_COMPLETE:
                Serial.println("Feed completed");   
                break;

            case CMD_SUCCESS:
                break;

            default:
                sendCmdPacket(CMD_ERROR,0);
                break;
        }
    }

    // Handle BLE credentials
    if(BLE_HasCredentials())
    {
        String ssid,password;

        if(BLE_GetCredentials(ssid,password))
        {
            SaveCredentials(ssid,password);

            if(WiFi_Connect(ssid,password))
            {
                BLE_Stop();
                sendCmdPacket(CMD_SUCCESS,1);
                Serial.println("Wifi initialized with BLE provision!");
            }
            else
            {
                sendCmdPacket(CMD_ERROR,1);
            }

            BLE_ClearCredentialFlag();
        }
    }
}
void handleStatus()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String json = "{";
    json += "\"foodLevel\":";
    json += String(foodLevel);
    json += ",\"lastFed\":\"";
    json += lastFed;
    json += "\"}";

    server.send(200, "application/json", json);
}
void handleFeed()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Feed request received from server");
    sendCmdPacket(CMD_FEED,1);
    Serial.println("Feed request sent to feeder");

    server.send(200,
                "application/json",
                "{\"success\":true}");
}