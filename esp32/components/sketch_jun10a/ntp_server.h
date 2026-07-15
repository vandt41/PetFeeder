#ifndef NTP_SERVER_H
#define NTP_SERVER_H
#include <time.h>
#include <Arduino.h>

// Use multiple NTP servers as fallbacks
static const char* ntpServers[] = {
  "pool.ntp.org",           // Global pool
  "asia.pool.ntp.org",      // Asia pool
  "vn.pool.ntp.org",        // Vietnam specific pool
  "time.google.com",        // Google's NTP
  "time.cloudflare.com"     // Cloudflare's NTP
};
bool obtainTime();
#endif