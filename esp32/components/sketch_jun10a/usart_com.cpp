#include "usart_com.h"

HardwareSerial MySerial(2);

void USART_Init()
{
    MySerial.begin(115200, SERIAL_8N1, RXD2, TXD2);

    Serial.printf(
        "UART2 initialized: RX=%d TX=%d baud=115200\r\n",
        RXD2,
        TXD2
    );

    // Flush any garbage already in RX buffer
    while (MySerial.available())
    {
        MySerial.read();
    }
}

void sendCmdPacket(Command_t cmd, uint8_t value)
{
    CommandPacket_t pkt;

    pkt.command = cmd;
    pkt.value   = value;

    Serial.printf(
        "TX: CMD=%d VALUE=%d | RAW=%02X %02X\r\n",
        (uint8_t)pkt.command,
        pkt.value,
        (uint8_t)pkt.command,
        pkt.value
    );

    MySerial.write(
        reinterpret_cast<uint8_t*>(&pkt),
        sizeof(pkt)
    );

    MySerial.flush();
}

bool readCmdPacket(CommandPacket_t &pkt)
{
    if (MySerial.available() < sizeof(CommandPacket_t))
    {
        return false;
    }

    uint8_t raw[sizeof(CommandPacket_t)];

    raw[0] = MySerial.read();
    raw[1] = MySerial.read();

    pkt.command = static_cast<Command_t>(raw[0]);
    pkt.value   = raw[1];

    Serial.printf(
        "RX: CMD=%d VALUE=%d | RAW=%02X %02X\r\n",
        raw[0],
        raw[1],
        raw[0],
        raw[1]
    );

    return true;
}