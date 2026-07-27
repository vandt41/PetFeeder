#include "ble_manager.h"
// #include "preferences_manager.h"
#include "wifi_manager.h"
// #include "ntp_server.h"
#include "stdint.h"
#include "usart_com.h"

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Starting...");
    
    USART_Init();
  
    delay(1000);
    while (MySerial.available())
        MySerial.read();
    

}

void loop() {

    CommandPacket_t pkt;

    if (readCmdPacket(pkt)) {
        Serial.println("PACKET RECEIVED");
        Serial.print("Command: ");
        Serial.println((Command_t)pkt.command);  
        Serial.print("Value: ");
        Serial.println(pkt.value);   

        // Handle based on command
        switch(pkt.command) {
            case CMD_SUCCESS:
                Serial.println("Command excecuted succesfully.");
                break;
            case CMD_INIT:
                // Turn on BLE and Wifi to receive credential from web app
                break;
            case CMD_FEED_COMPLETE:
                // update web app data
                break;
            default:
                Serial.println("Unknown command!");
                sendCmdPacket(CMD_ERROR, 0);
                break;
        }
    }
}