#include "usart_com.h"
HardwareSerial MySerial(1);

void USART_Init()
{
    MySerial.begin(115200, SERIAL_8N1, RXD1, TXD1);
}
void sendStatusPacket(uint8_t bleStatus, uint8_t wifiStatus)
{
    StatusPacket_t pkt;

    pkt.command = CMD_STATUS_RESPONSE;
    pkt.bleStatus = bleStatus;
    pkt.wifiStatus = wifiStatus;

    MySerial.write((uint8_t *)&pkt, sizeof(pkt));
}
void sendTimePacket(Command_t cmd, uint32_t unixTime)
{
    TimePacket_t tmPkt;
    tmPkt.unixTime = unixTime;
    tmPkt.command = cmd;
    MySerial.write((uint8_t*)&tmPkt, sizeof(tmPkt));
}
void sendCmdPacket(Command_t cmd, uint8_t value)
{
    CommandPacket_t pkt;
    pkt.value = value;
    pkt.command = cmd;
    MySerial.write((uint8_t*)&pkt, sizeof(pkt));
}
bool readCmdPacket(CommandPacket_t &pkt)
{
    if (MySerial.available() >= sizeof(CommandPacket_t))
    {
        MySerial.readBytes((uint8_t*)&pkt, sizeof(CommandPacket_t));
        return true;
    }
    return false;
}