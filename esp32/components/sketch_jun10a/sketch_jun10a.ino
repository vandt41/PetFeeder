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
    delay(100);

    Serial.println();
    Serial.println("ESP32 starting...");

    USART_Init();

    if (WiFi_ConnectSaved())
    {
        Serial.println("Connected using saved credentials");

        openWebServer();

        server.begin();

        Serial.println("Web server started");
        Serial.println(WiFi.localIP());

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
    sendCmdPacket(CMD_STATUS_REQUEST, 0);
}

void loop()
{
    server.handleClient();

    // if (millis() - lastStatusRequest >= STATUS_REFRESH_INTERVAL)
    // {
    //     requestSTM32Status();
    //     lastStatusRequest = millis();
    //     Serial.println("Periodic status refresh requested");
    // }

    CommandPacket_t pkt;
    if(readCmdPacket(pkt))
    {
        switch(pkt.command)
        {
            case CMD_INIT:
                Serial.println("Enter BLE init process.\n");
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
                Serial.printf("Unknown STM32 command: 0x%02X\r\n", (uint8_t)pkt.command);
                break;
        }
    }

if (BLE_HasCredentials())
{
    String ssid;
    String password;

    if (BLE_GetCredentials(ssid, password))
    {
        // Consume credentials immediately
        BLE_ClearCredentialFlag();

        Serial.println("Credentials received");
        Serial.println("Connecting to WiFi...");

        SaveCredentials(ssid, password);

        if (WiFi_Connect(ssid, password))
        {
            sendCmdPacket(CMD_INIT_COMPLETE, 24);
            openWebServer();
            Serial.println("WiFi connected!");
            BLE_Stop();
        }
        else
        {
            Serial.println("WiFi connection failed");
            sendCmdPacket(CMD_ERROR, ERR_TIMEOUT_WIFI);
        }
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

    server.send(200,
                "application/json",
                "{\"success\":true}");
}
void requestSTM32Status()
{
    sendCmdPacket(CMD_STATUS_REQUEST, 0);
    // The response will be handled by the CMD_STATUS_RESPOND case in loop()
}
void openWebServer(void){
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/feed", HTTP_POST, handleFeed);

    server.on("/status", HTTP_OPTIONS, []()
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
    });

    server.on("/feed", HTTP_OPTIONS, []()
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
});
}