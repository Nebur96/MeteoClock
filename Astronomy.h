#include <WString.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#ifndef Astronomy_h
#define Astronomy_h

class Astronomy
{
  
  public:
    Astronomy(const char* LAT, const char* LNG);
    Astronomy(const char* LAT, const char* LNG, const int SUNRISE_HOUR, const int SUNSET_HOUR);
    boolean updateSunriseAndSunsetHours();

    int sunriseHour;
    int sunsetHour;

  private:
    const char* _LAT;
    const char* _LNG;
    const char* _HOST = "api.sunrise-sunset.org";
    const int _HTTP_PORT = 80;
    //HOURS CONVERSION AFTER 12 AM FOR 24 HOURS CLOCK
    const int hoursPM[12] = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 12};
};

#endif
