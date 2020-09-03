#include <WString.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#ifndef Weather_h
#define Weather_h

#define URL_LENGHT 136
#define REQUEST_PARAMETERS_LENGHT 16
//TODO this value should be review! the variable size is 197 but it does not work when it is declared with a value bellow than this
#define HTTP_REQUEST_LENGHT 200

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

  typedef enum
  {
    Fahrenheit = 'F',
    Celsius = 'C',
    Kelvin
  } WeatherUnit;

public:
  Weather(const char *OPEN_WEATHER_KEY, const char *LAT, const char *LNG);
  boolean getCurrentWeatherConditions();
  boolean getWeatherForecast();
  Condition conditions;
  Forecast forecasts[3];

private:
  WiFiClient client;
  char *buildRequestURL();
  char *buildUnitsRequestParameter();
  char *buildHttpRequest();
  
  String truncateTemperatureValue(String temperature);
  
  int calculateDayOfWeek(long unixTimestamp);

  String performHttpRequest();

  const int _HTTP_PORT = 80;
  const char *_HOST = "api.openweathermap.org";

  const char *_URL;

  const int _NUMBER_OF_FORECASTS = 3;

  const char *_OPEN_WEATHER_KEY;
  const char *_LAT;
  const char *_LNG;

  WeatherUnit _UNIT = Celsius;

  //DAYS OF WEEK NAMES
  const char* daysOfWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};

#endif