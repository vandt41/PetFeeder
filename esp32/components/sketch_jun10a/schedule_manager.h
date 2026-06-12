#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <Preferences.h>
#include <time.h>

#define MAX_SCHEDULES 5

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t portion;
    bool enabled;
} FeedSchedule_t;

static FeedSchedule_t schedules[MAX_SCHEDULES];

static Preferences schedPrefs;

static void Schedule_Save()
{
    schedPrefs.begin("schedule", false);

    schedPrefs.putBytes(
        "table",
        schedules,
        sizeof(schedules));

    schedPrefs.end();
}

static void Schedule_Load()
{
    schedPrefs.begin("schedule", true);

    schedPrefs.getBytes(
        "table",
        schedules,
        sizeof(schedules));

    schedPrefs.end();
}

static bool Schedule_Check(
    FeedSchedule_t* triggered)
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
    {
        return false;
    }

    static int lastMinute = -1;

    if(lastMinute ==
       timeinfo.tm_min)
    {
        return false;
    }

    lastMinute =
        timeinfo.tm_min;

    for(int i = 0;
        i < MAX_SCHEDULES;
        i++)
    {
        if(!schedules[i].enabled)
            continue;

        if(schedules[i].hour ==
               timeinfo.tm_hour &&
           schedules[i].minute ==
               timeinfo.tm_min)
        {
            *triggered =
                schedules[i];

            return true;
        }
    }

    return false;
}

#endif