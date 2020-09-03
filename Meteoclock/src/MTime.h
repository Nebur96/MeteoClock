#include <time.h>
#include <Arduino.h>
#include <sys/time.h>
#include <coredecls.h>

#ifndef Time_h
#define Time_h

class MTime
{
public:
    MTime(unsigned int TZ_SEC, unsigned int DST_SEC, const char *NTP_POOL);
    void initializeTime();
    void time_is_set(void);
    void loop();
    unsigned int hour();
    unsigned int minute();
    unsigned int day();
    unsigned int month();
    const char *monthName();
    unsigned int year();
    const char *weekdayName();
    unsigned int weekday();

private:
    const char *_NTP_POOL;
    unsigned int _TZ_SEC;
    unsigned int _DST_SEC;

    const unsigned int BASE_YEAR = 1900;

    //DAYS OF WEEK NAMES
    const char *daysOfWeek[7] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
    //MONTHS NAMES
    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

    timeval _cbtime;
    bool _cbtime_set = false;
    time_t _now;
    tm *_tm;
};

#endif