#ifndef PREFERENCES_MANAGER_H
#define PREFERENCES_MANAGER_H
#include <Preferences.h>
typedef struct
{
    String ssid;
    String password;
}WifiCredentials_t;
static Preferences preferences;
bool SaveCredentials(const String& ssid, const String& password);
void ClearCredentials();
bool LoadCredentials(WifiCredentials_t& creds);

#endif