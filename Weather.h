#include <WString.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <stdlib.h>

#ifndef WundergroundClient_h
#define WundergroundClient_h

class Weather
{
    typedef struct
    {
      String weather;
      String icon;
      String temperature;
    } Condition;

    typedef struct
    {
      String high_temperature;
      String low_temperature;
      String weekDay;
      String icon;
    } Forecast;

  public:
    Weather(char* WUNDERGROUND_KEY, char* COUNTRY_CODE, char* CITY_CODE, int NUMBER_OF_FORECASTS);
    void getCurrentWeatherConditions();
    void getWeatherForecast();
    Condition conditions;
    Forecast forecasts[3];
  private:
    WiFiClient client;

    const int _HTTP_PORT = 80;
    const char * _HOST = "api.wunderground.com";

    char* _WUNDERGROUND_KEY;
    char* _COUNTRY_CODE;
    char* _CITY_CODE;
    
    int _NUMBER_OF_FORECASTS;
};

#endif
