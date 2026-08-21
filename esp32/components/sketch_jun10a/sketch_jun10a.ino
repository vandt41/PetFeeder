#include "ble_manager.h"
#include "preferences_manager.h"
#include "wifi_manager.h"
// #include "ntp_server.h"
#include "stdint.h"
#include "usart_com.h"
#include <WebServer.h>
#define STATUS_FEEDER  (1U << 0)
#define STATUS_UART    (1U << 1)
#define STATUS_LED     (1U << 2)
#define STATUS_BUTTON  (1U << 3)
#define STATUS_RTC     (1U << 4)
#define STATUS_ERROR   (1U << 5)
WebServer server(80);

uint8_t stm32Status = 0;
uint8_t wifiStatus = 0;
uint8_t bleStatus = 0;

// Status refresh timing
unsigned long lastStatusRequest = 0;
const unsigned long STATUS_REFRESH_INTERVAL = 30000; // 30 seconds

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
        requestSTM32Status();
        lastStatusRequest = millis();
    }
    if (WiFi_IsConnected())
    {
        Serial.println("Connected!");
        wifiStatus = 1;
    }
    else
    {
        Serial.println("Not connected");
        wifiStatus = 0;
    }
}

void loop()
{
    server.handleClient();

    if (millis() - lastStatusRequest >= STATUS_REFRESH_INTERVAL)
    {
        requestSTM32Status();
        lastStatusRequest = millis();
        Serial.println("Periodic status refresh requested");
    }

    CommandPacket_t pkt;
    if(readCmdPacket(pkt))
    {
        switch(pkt.command)
        {
            case CMD_INIT:
                BLE_Init();
                break;
            case CMD_DEINIT:  
                BLE_Stop();
                break;

            case CMD_FEED_COMPLETE:
            {
                uint8_t portion = pkt.value;
                Serial.print("Feed completed with the amount of portion: ");
                Serial.println(portion);
                requestSTM32Status();
                lastStatusRequest = millis();
                break;
            }
                
            case CMD_STATUS_RESPOND:
                stm32Status = pkt.value;
                Serial.println("STM32 status updated");
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

    bool feeder = stm32Status & STATUS_FEEDER;
    bool uart   = stm32Status & STATUS_UART;
    bool led    = stm32Status & STATUS_LED;
    bool button = stm32Status & STATUS_BUTTON;
    bool rtc    = stm32Status & STATUS_RTC;
    bool error  = stm32Status & STATUS_ERROR;
    bool wifi   = WiFi_IsConnected();
    bool ble    = BLE_IsRunning();

    String json = "{";
    json += "\"feeder\":";
    json += feeder ? "true" : "false";
    json += ",\"uart\":";
    json += uart ? "true" : "false";
    json += ",\"led\":";
    json += led ? "true" : "false";
    json += ",\"button\":";
    json += button ? "true" : "false";
    json += ",\"rtc\":";
    json += rtc ? "true" : "false";
    json += ",\"error\":";
    json += error ? "true" : "false";
    json += ",\"wifi\":";
    json += wifi ? "true" : "false";
    json += ",\"ssid\":\"";
    json += wifi ? WiFi.SSID() : "";
    json += "\"";
    json += ",\"ble\":";
    json += ble ? "true" : "false";
    json += "}";

    server.send(200, "application/json", json);
}
void handleFeed()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Feed request received from server");
    sendCmdPacket(CMD_FEED,1);
    Serial.println("Feed request sent to feeder");
    requestSTM32Status();

    server.send(200,
                "application/json",
                "{\"success\":true}");
}
void requestSTM32Status()
{
    sendCmdPacket(CMD_STATUS_REQUEST, 0);
    // The response will be handled by the CMD_STATUS_RESPOND case in loop()
}