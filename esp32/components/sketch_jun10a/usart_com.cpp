#include "usart_com.h"
HardwareSerial MySerial(2);

void USART_Init()
{    // Initialize UART2 with pull-up enabled
    pinMode(RXD2, INPUT_PULLUP);
    MySerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
    
    Serial.println("UART2 initialized with pull-up on RX pin 16");
    
    while (MySerial.available())
        MySerial.read();     // Flush RX FIFO
}

// void sendTimePacket(Command_t cmd, uint32_t unixTime)
// {
//     TimePacket_t tmPkt;
//     tmPkt.unixTime = unixTime;
//     tmPkt.command = cmd;
//     MySerial.write((uint8_t*)&tmPkt, sizeof(tmPkt));
// }
void sendCmdPacket(Command_t cmd, uint8_t value)
{
    CommandPacket_t pkt;
    pkt.value = value;
    pkt.command = cmd;
    Serial.printf("TX: %02X %02X\n",
              (uint8_t)pkt.command,
              pkt.value);
    MySerial.write((uint8_t*)&pkt, sizeof(pkt));
}

bool readCmdPacket(CommandPacket_t &pkt)
{
    if (MySerial.available() >= sizeof(CommandPacket_t))
    {
        uint8_t raw[2];

        MySerial.readBytes(raw, 2);

        Serial.printf("RAW: %02X %02X\n", raw[0], raw[1]);

        memcpy(&pkt, raw, 2);

        return true;
    }

    return false;
}


// bool readCmdPacket(CommandPacket_t &pkt) {
//     if (MySerial.available() >= sizeof(CommandPacket_t)) {
//         uint8_t buffer[sizeof(CommandPacket_t)];
//         MySerial.readBytes(buffer, sizeof(CommandPacket_t));
        
//         // ✅ Read byte by byte to avoid alignment issues
//         pkt.command = (Command_t)buffer[0];
//         pkt.value = buffer[1];
//         return true;
//     }
//     return false;
// }