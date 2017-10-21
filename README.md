﻿# MeteoClock

MeteoClock is an arduino based clock that displays current date, hours, weather information and forecast for the next 2 days. It requires internet connection to run properly and get all information from webservices. Every half hour it updates the current time from a NTP Server and the current weather as well. At midnight it refresh the whole screen updating all the information it displays.

![Alt text](/resources/IMG_20171021_193103.jpg?raw=true "MeteoClock Preview")


MeteoClock features:

- display current date  -> ✓
- display current hours  -> ✓
- display current weather  -> ✓
- display forecast for next 2 days  -> ✓
- display correct icons according to weather condition  -> ✓
- get correct sunrise and sunset hours by gps coordinate to change between day and night icons  -> ✓
- create web interface to interact with MeteoClock and change user definitions without have to change it on code
- create Auto-Management for WiFi Connections
