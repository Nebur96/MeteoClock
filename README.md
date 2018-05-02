# MeteoClock

MeteoClock is an arduino based clock that displays current date, hours, weather information and forecast for the next 2 days. It requires internet connection to run properly and get all information from webservices. Every half hour it updates the current time from a NTP Server and the current weather as well. At midnight it refresh the whole screen updating all the information it displays. It uses a nodeMCU(ESP8266) and a 2.2" Serial Port TFT SPI LCD Screen Module with ILI9342 driver.

![alt-text-1](/demo/day_picture.jpg "MeteoClock Day Preview") ![alt-text-2](/demo/night_picture.jpg "MeteoClock Night Preview")

# Implemented Features:

- display current date
- display current time
- display current weather
- display weather forecast (next 2 days)
- display correct icons according to weather condition
- get correct sunrise and sunset hours by gps coordinate
- change icons between day and night icon sets 
- WiFi connection manager
- daylight saving time

# Setup:

Install the following libraries:
- Adafruit_GFX
- Adafruit_ILI9341
- ArduinoJson
- ESP8266WiFi
- DNSServer
- ESP8266WebServer
- WiFiManager
- TimeLib
- ArduinoTimerObject

Edit the file "config.h" and enter your values:
- WUNDERGROUD_API -> your Wunderground API
- WUNDERGROUND_COUNTRY -> your country Wunderground reference
- WUNDERGROUND_CITY -> your city Wunderground reference
- LAT -> latitude of your location
- LNG -> longitude of your location
- SUNSET_HOURS -> your default hours for sunset 
- SUNRISE_HOURS -> your default hours for sunrise
- NTP_POOL -> NTP pool to retrieve time
- TZ -> (utc+) in hours
- DST_MN -> use 60mn for summer time in some countries
