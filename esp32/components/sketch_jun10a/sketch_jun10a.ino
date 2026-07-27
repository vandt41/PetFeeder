#include "ble_manager.h"
#include "preferences_manager.h"
#include "wifi_manager.h"
// #include "ntp_server.h"
#include "stdint.h"
#include "usart_com.h"

void setup()
{
    Serial.begin(115200);

    USART_Init();

    if(WiFi_ConnectSaved())
    {
        Serial.println("Connected using saved credentials");
    }
}

void loop()
{
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