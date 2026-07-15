#include "preferences_manager.h"


bool SaveCredentials(const String& ssid,
                            const String& password)
{
    preferences.begin("wifi", false);

    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    preferences.end();

    Serial.println("Credentials saved");

    return true;
}
bool LoadCredentials(WifiCredentials_t& creds)
{
    preferences.begin("wifi", true);

    creds.ssid =
        preferences.getString("ssid", "");

    creds.password =
        preferences.getString("password", "");

    preferences.end();

    return (creds.ssid.length() > 0);
}

void ClearCredentials()
{
    preferences.begin("wifi", false);

    preferences.clear();

    preferences.end();
}
