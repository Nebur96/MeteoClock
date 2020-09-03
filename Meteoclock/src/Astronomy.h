#include <WString.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#ifndef Astronomy_h
#define Astronomy_h

#define REQUEST_URI_SIZE 30
#define REQUEST_SIZE 105

class Astronomy
{
public:
  Astronomy(const char *LAT, const char *LNG);
  Astronomy(const char *LAT, const char *LNG, const int SUNRISE_HOUR, const int SUNSET_HOUR);
  boolean updateSunriseAndSunsetHours();

  int sunriseHour;
  int sunsetHour;

private:
  WiFiClient client;
  char *buildRequestURL();
  char *buildHttpRequest();
  String performHttpRequest();

  const char *_LAT;
  const char *_LNG;

  const char *_HOST = "api.sunrise-sunset.org";
  const int _HTTP_PORT = 80;

  const char *_URL;

  //HOURS CONVERSION AFTER 12 AM FOR 24 HOURS CLOCK
  const int hoursPM[12] = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 12};
};

#endif
