#include "ble_manager.h"
#include "preferences_manager.h"
#include "wifi_manager.h"
#include "ntp_server.h"
#include "usart_com.h"
#include "stdint.h"
uint8_t food_level = 0;
bool wifi_update_requested = false;
String ssid;
String password;

void HandleFeedEvent(uint8_t val);
void setup()
{
    Serial.begin(115200);
    if(WiFi_ConnectSaved())
    {
        Serial.println("WiFi OK");
        obtainTime();
    }
    else
    {
        BLE_Init();
    }

    USART_Init();
}
void loop()
{
    if(wifi_update_requested)
    {
        if (BLE_HasCredentials())
        {
            SaveCredentials(ssid, password);
            BLE_ClearCredentialFlag();

            if(WiFi_ConnectSaved())
            {
                obtainTime();
                BLE_Stop();
                wifi_update_requested = false;
            }
        }
    }
    CommandPacket_t pkt;
    if(readCmdPacket(pkt)){
        switch (pkt.command){
            case CMD_FEED:
                HandleFeedEvent(pkt.value);
                sendCmdPacket(CMD_FEED_COMPLETE, pkt.value);
                break;
            case CMD_UPDATE_WIFI:
                if (!wifi_update_requested)
                {
                    BLE_Init();
                    wifi_update_requested = true;
                }
                break;
            // send ntp time info to update stm32's rtc
            case CMD_NTP_TIME_REQUEST:
            {
                time_t now;
                time(&now);

                sendTimePacket(CMD_NTP_TIME_RESPONSE, static_cast<uint32_t>(now));
                break;
            }
            case CMD_STATUS_REQUEST:
            {
                sendStatusPacket(BLE_IsRunning(), WiFi.status() == WL_CONNECTED);
                break;
            }
            // receive food level from stm32's sensor
            case CMD_STATUS_RESPONSE:
                food_level = pkt.value;
                break;
            // send error
            default:
                Serial.println("Unknown command");
                break;
        }
        
    }
}
    void HandleFeedEvent(uint8_t val)
    {
    // do nothing for now
    return;
    }