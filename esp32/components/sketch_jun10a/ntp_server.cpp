#include "ntp_server.h"

static const long gmtOffset_sec = 25200;   // UTC+7 for Vietnam
static const int daylightOffset_sec = 0;

bool obtainTime() {
  struct tm timeinfo;
  // Configure with first server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[0]);
  
  int attempts = 0;
  const int maxAttempts = 30; // More attempts for reliability
  
  Serial.println("Syncing time with NTP servers...");
  
  while (!getLocalTime(&timeinfo) && attempts < maxAttempts) {
    // Try different servers after 10 failed attempts
    if (attempts == 10) {
      Serial.println("\nSwitching to backup NTP server...");
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[1]);
    } else if (attempts == 15) {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[2]);
    } else if (attempts == 20) {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[3]);
    } else if (attempts == 25) {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[4]);
    }
    
    Serial.print(".");
    delay(1000);
    attempts++;
  }
  
  if (attempts < maxAttempts) {
    Serial.println("\n✓ Time synchronized successfully!");
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    return true;
  } else {
    Serial.println("\n❌ Failed to obtain time from all NTP servers");
    Serial.println("Check WiFi connection and try again");
    return false;
  }
}