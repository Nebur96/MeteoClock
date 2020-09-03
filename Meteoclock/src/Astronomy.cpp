#include "Astronomy.h"

Astronomy::Astronomy(const char *LAT, const char *LNG)
{
    _LAT = LAT;
    _LNG = LNG;

    _URL = buildRequestURL();
}

Astronomy::Astronomy(const char *LAT, const char *LNG, const int SUNRISE_HOUR, const int SUNSET_HOUR)
{
    _LAT = LAT;
    _LNG = LNG;
    sunriseHour = SUNRISE_HOUR;
    sunsetHour = SUNSET_HOUR;

    _URL = buildRequestURL();
}

boolean Astronomy::updateSunriseAndSunsetHours()
{
    String response = performHttpRequest();
    if (response.length() == 0)
    {
        return true;
    }

    DynamicJsonBuffer jsonBuffer;

    JsonObject &root = jsonBuffer.parseObject(response);
    jsonBuffer.clear();

    if (root["status"] == "OK")
    {
        JsonObject &results = root["results"];

        String sunriseHour = results["sunrise"].as<String>();
        sunriseHour = sunriseHour.substring(0, 1).toInt();

        String sunsetHour = results["sunset"].as<String>();
        sunsetHour = hoursPM[sunsetHour.substring(0, 1).toInt() - 1];
    }

    return false;
}

char *Astronomy::buildRequestURL()
{
    char *requestURL = (char *)malloc(REQUEST_URI_SIZE);
    sprintf(requestURL, "/json?lat=%s&lng=%s", _LAT, _LNG);
    return requestURL;
}

char *Astronomy::buildHttpRequest()
{
    char *request = (char *)malloc(REQUEST_SIZE);
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", _URL, _HOST);
    return request;
}

String Astronomy::performHttpRequest()
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
            Serial.println("Client Timeout!");
            client.stop();
            return "";
        }
    }

    String response = "";
    while (client.available())
    {
        response += client.readStringUntil('\r');
    }

    return response;
}