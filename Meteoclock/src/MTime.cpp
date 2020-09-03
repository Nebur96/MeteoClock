#include "MTime.h"

MTime::MTime(unsigned int TZ_SEC, unsigned int DST_SEC, const char *NTP_POOL)
{
    Serial.begin(115200);

    _TZ_SEC = TZ_SEC;
    _DST_SEC = DST_SEC;
    _NTP_POOL = NTP_POOL;
}

void MTime::initializeTime()
{
    settimeofday_cb([=](){return this->time_is_set();});
    configTime(_TZ_SEC, _DST_SEC, _NTP_POOL);

    while (!_cbtime_set)
    {
        delay(100);
    }

    loop();
}

void MTime::time_is_set(void)
{
    gettimeofday(&_cbtime, nullptr);
    _now = time(nullptr);

    _cbtime_set = true;
}

void MTime::loop() {
    gettimeofday(&_cbtime, nullptr);
    _now = time(nullptr);
    _tm = localtime(&_now);
}

unsigned int MTime::hour()
{
    return _tm->tm_hour;
}

unsigned int MTime::minute()
{
    return _tm->tm_min;
}

unsigned int MTime::day()
{
    return _tm->tm_mday;
}

unsigned int MTime::month()
{
    return _tm->tm_mon;
}

const char *MTime::monthName()
{
    return months[month()];
}

unsigned int MTime::year()
{
    return BASE_YEAR + _tm->tm_year;
}

unsigned int MTime::weekday()
{
    return _tm->tm_wday;
}

const char *MTime::weekdayName()
{
    return daysOfWeek[weekday()];
}