#include "ble_manager.h"
#include "preferences_manager.h"
#include "wifi_manager.h"
#include "schedule_manager.h"
#include "ntp_server.h"
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

    Schedule_Load();
}
void loop()
{
    if(BLE_HasCredentials())
    {
        BLE_ClearCredentialFlag();

        if(WiFi_ConnectSaved())
        {
            obtainTime();
            BLE_Stop();
        }
    }

    FeedSchedule_t sched;

    if(Schedule_Check(&sched))
    {
        Serial.printf("FEED,%d\n",
                      sched.portion);
    }
}