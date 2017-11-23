#include "Astronomy.h"

Astronomy::Astronomy(const char* LAT, const char* LNG)
{
  _LAT = LAT;
  _LNG = LNG;
}

Astronomy::Astronomy(const char* LAT, const char* LNG, const int SUNRISE_HOUR, const int SUNSET_HOUR)
{
  _LAT = LAT;
  _LNG = LNG;
  sunriseHour = SUNRISE_HOUR;
  sunsetHour = SUNSET_HOUR;
}

boolean Astronomy::updateSunriseAndSunsetHours()
{
  WiFiClient client;

  String response = "";

  if (!client.connect(_HOST, _HTTP_PORT))
  {
    Serial.println("connection failed");
    return true;
  }

  String url = "/json?lat=";
  url += _LAT;
  url += "&lng=";
  url += _LNG;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + _HOST + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Sunrise and Sunset Client Timeout!");
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

  if (root["status"] == "OK") {
    String result = root["results"]["sunrise"].as<String>();
    sunriseHour = result.substring(0, 1).toInt();

    result = root["results"]["sunset"].as<String>();
    sunsetHour = hoursPM[result.substring(0, 1).toInt() - 1];
  }

  return false;
}
