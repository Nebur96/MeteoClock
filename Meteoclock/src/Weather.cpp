#include "Weather.h"

Weather::Weather(const char *OPEN_WEATHER_KEY, const char *LAT, const char *LNG)
{
  Serial.begin(115200);
  _OPEN_WEATHER_KEY = OPEN_WEATHER_KEY;
  _LAT = LAT;
  _LNG = LNG;

  _URL = buildRequestURL();

  conditions.weather = "NaN";
  conditions.temperature = "NaN";
  conditions.icon = "none";

  for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i)
  {
    forecasts[i].weekDay = "NaN";
    forecasts[i].icon = "none";
    forecasts[i].high_temperature = "NaN";
    forecasts[i].low_temperature = "NaN";
  }
}

boolean Weather::getCurrentWeatherConditions()
{
  String rawResponse = performHttpRequest();
  if (rawResponse.length() == 0)
  {
    return true;
  }

  DynamicJsonBuffer jsonBuffer;

  JsonObject &jsonResponse = jsonBuffer.parseObject(rawResponse);
  jsonBuffer.clear();

  JsonObject &current_day = jsonBuffer.parseObject(jsonResponse["daily"][0].as<String>());
  jsonBuffer.clear();

  if (!current_day.success())
  {
    Serial.println("not sucess");
    conditions.weather = "NaN";
    conditions.temperature = "NaN";
    conditions.icon = "none";
    return true;
  }

  conditions.temperature = current_day["temp"]["day"].as<String>();

  JsonObject &weather = current_day["weather"][0];
  conditions.weather = weather["main"].as<String>();
  conditions.icon = weather["icon"].as<String>();

  return false;
}

boolean Weather::getWeatherForecast()
{
  String rawResponse = performHttpRequest();
  if (rawResponse.length() == 0)
  {
    return true;
  }

  DynamicJsonBuffer jsonBuffer;

  JsonObject &jsonResponse = jsonBuffer.parseObject(rawResponse);
  jsonBuffer.clear();

  if (!jsonResponse.success())
  {
    for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i)
    {
      forecasts[i].weekDay = "NaN";
      forecasts[i].icon = "none";
      forecasts[i].high_temperature = "NaN";
      forecasts[i].low_temperature = "NaN";
    }

    return true;
  }

  for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i)
  {
    JsonObject &daily = jsonResponse["daily"][i];

    forecasts[i].weekDay = daysOfWeek[calculateDayOfWeek(daily["dt"].as<long>())];

    forecasts[i].icon = daily["weather"][0]["icon"].as<String>();

    JsonObject &daily_temp = daily["temp"];
    forecasts[i].high_temperature = truncateTemperatureValue(daily_temp["max"].as<String>());
    forecasts[i].low_temperature = truncateTemperatureValue(daily_temp["min"].as<String>());
  }

  return false;
}

char *Weather::buildRequestURL()
{
  char *requestURL = (char *)malloc(URL_LENGHT);
  sprintf(requestURL, "/data/2.5/onecall?lat=%s&lon=%s&exclude=hourly,current,minutely&appid=%s%s", _LAT, _LNG, _OPEN_WEATHER_KEY, buildUnitsRequestParameter());
  return requestURL;
}

char *Weather::buildUnitsRequestParameter()
{
  char *unitParameter = (char *)malloc(REQUEST_PARAMETERS_LENGHT);
  switch (_UNIT)
  {
  case Fahrenheit:
    sprintf(unitParameter, "&units=%s", "imperial");
  case Celsius:
    sprintf(unitParameter, "&units=%s", "metric");
  case Kelvin:
  default:
    return unitParameter;
  }

  return unitParameter;
}

char *Weather::buildHttpRequest()
{
  char *request = (char *)malloc(HTTP_REQUEST_LENGHT);
  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", _URL, _HOST);
  return request;
}

String Weather::performHttpRequest()
{
  if (!client.connect(_HOST, _HTTP_PORT))
  {
    Serial.println("connection failed");
    return "";
  }

  client.print(buildHttpRequest());

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println("Weather -> Forecast >>> Client Timeout!");
      client.stop();
      return "";
    }
  }

  String response = "";
  while (client.available())
  {
    response += client.readStringUntil('\r');
  }

  if (response.length() == 0)
  {
    return "";
  }
  response = response.substring(response.indexOf("{"));

  return response;
}

String Weather::truncateTemperatureValue(String temperature)
{
  return temperature.substring(0, temperature.lastIndexOf('.'));
}

int Weather::calculateDayOfWeek(long unixTimestamp)
{
  return ((unixTimestamp / 86400) + 4) % 7;
}