#include "Weather.h"

Weather::Weather(char* WUNDERGROUND_KEY, char* COUNTRY_CODE, char* CITY_CODE, int NUMBER_OF_FORECASTS)
{
  Serial.begin(115200);
  _WUNDERGROUND_KEY = WUNDERGROUND_KEY;
  _COUNTRY_CODE = COUNTRY_CODE;
  _CITY_CODE = CITY_CODE;
  _NUMBER_OF_FORECASTS = NUMBER_OF_FORECASTS;

  conditions.weather = "NaN";
  conditions.temperature = "NaN";
  conditions.icon = "none";

  for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i) {
    forecasts[i].weekDay = "NaN";
    forecasts[i].icon = "none";
    forecasts[i].high_temperature = "NaN";
    forecasts[i].low_temperature = "NaN";
  }
}

boolean Weather::getCurrentWeatherConditions()
{
  String response = "";

  if (!client.connect(_HOST, _HTTP_PORT))
  {
    Serial.println("connection failed");
    return true;
  }

  String url = "/api/";
  url += _WUNDERGROUND_KEY;
  url += "/conditions/q/";
  url += _COUNTRY_CODE + String("/");
  url += _CITY_CODE + String(".json");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + _HOST + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println("Weather -> Current Condition >>>  Client Timeout!");
      client.stop();
      return true;
    }
  }

  while (client.available()) {
    response += client.readStringUntil('\r');
  }

  response = response.substring(response.indexOf("{"));

  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(response);
  jsonBuffer.clear();
  JsonObject& current_observation = jsonBuffer.parseObject(root["current_observation"].as<String>());
  jsonBuffer.clear();

  if (!current_observation.success()) {
    conditions.weather = "NaN";
    conditions.temperature = "NaN";
    conditions.icon = "none";
    return true;
  }

  conditions.weather = current_observation["weather"].as<String>();
  conditions.temperature = current_observation["temp_c"].as<String>();
  conditions.icon = current_observation["icon"].as<String>();

  return false;
}

boolean Weather::getWeatherForecast()
{
  String response = "";

  if (!client.connect(_HOST, _HTTP_PORT))
  {
    Serial.println("connection failed");
    return true;
  }

  String url = "/api/";
  url += _WUNDERGROUND_KEY;
  url += "/forecast/q/";
  url += _COUNTRY_CODE + String("/");
  url += _CITY_CODE + String(".json");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + _HOST + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println("Weather -> Forecast >>> Client Timeout!");
      client.stop();
      return true;
    }
  }

  while (client.available()) {
    response += client.readStringUntil('\r');
  }

  response = response.substring(response.indexOf("{"));

  DynamicJsonBuffer jsonBuffer;

  response = response.substring(response.lastIndexOf("forecastday"));

  response = "{\"" + response.substring(0, response.length() - 2);

  JsonObject& root = jsonBuffer.parseObject(response);
  jsonBuffer.clear();

  if (!root.success()) {
    for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i) {
      forecasts[i].weekDay = "NaN";
      forecasts[i].icon = "none";
      forecasts[i].high_temperature = "NaN";
      forecasts[i].low_temperature = "NaN";
    }

    return true;
  }

  for (int i = 0; i < _NUMBER_OF_FORECASTS; ++i) {
    forecasts[i].weekDay = root["forecastday"][i]["date"]["weekday"].as<String>();
    forecasts[i].icon =  root["forecastday"][i]["icon"].as<String>();
    forecasts[i].high_temperature = root["forecastday"][i]["high"]["celsius"].as<String>();
    forecasts[i].low_temperature =  root["forecastday"][i]["low"]["celsius"].as<String>();
  }

  return false;
}
