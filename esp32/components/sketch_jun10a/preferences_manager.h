#ifndef PREFERENCES_MANAGER_H
#define PREFERENCES_MANAGER_H

#include <Preferences.h>

static Preferences preferences;

struct WifiCredentials
{
    String ssid;
    String password;
};

static bool SaveCredentials(const String& ssid,
                            const String& password)
{
    preferences.begin("wifi", false);

    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    preferences.end();

    Serial.println("Credentials saved");

    return true;
}

static bool LoadCredentials(WifiCredentials& creds)
{
    preferences.begin("wifi", true);

    creds.ssid =
        preferences.getString("ssid", "");

    creds.password =
        preferences.getString("password", "");

    preferences.end();

    return (creds.ssid.length() > 0);
}

static void ClearCredentials()
{
    preferences.begin("wifi", false);

    preferences.clear();

    preferences.end();
}

#endif