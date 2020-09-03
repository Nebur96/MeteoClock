//SCREEN LIBRARIES AND GRAPHICS
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "GfxUi.h"
#include "Image.h"

//JSON LIBRARY
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

//WIFI CONNECTION LIBRARIES
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

//TIME LIBRARIES
#include "MTime.h"

//TIMER LIBRARIES
#include <TimerObject.h> //https://github.com/aron-bordin/ArduinoTimerObject

//WEATHER LIBRARY
#include "Weather.h"

//ASTRONOMY LIBRARY
//#include "Astronomy.h"

//PROJECT CONSTANTS AND EXTERNAL VARIABLES
#include "Configs.h"

#define DATE_SIZE 20
#define HOUR_SIZE 6

const char *HOSTNAME = "MeteoClock";
const char *LOADING_MESSAGE = "Loading info...";

const char *NO_CONNECTION_MESSAGE_1 = "No connection,";
const char *NO_CONNECTION_MESSAGE_2 = "configure a new AP!";

const char *CONNECTION_TO_NTP_SERVER_MESSAGE_1 = "Waiting NTP server";
const char *CONNECTION_TO_NTP_SERVER_MESSAGE_2 = "response";

// LCD PINS CONNECTIONS
#define TFT_CS D2
#define TFT_DC D1
#define TFT_MOSI D7
#define TFT_CLK D5
#define TFT_RST D4
#define TFT_MISO D6

//WEATHER CONSTS
#define NUMBER_OF_FORECAST_PREDICTIONS 3

//WiFi CONSTS
#define MAX_WIFI_CONNECTIONS_ATTEMPTS 20

WiFiManager wifiManager;

typedef struct
{
    bool currentConditionsError;
    bool forecastConditionsError;
} Errors;

/********** FUNCTIONS DECLARATION **********/
void setDownloadScreen();
void setInitializationScreen();
void updateDateScreen();
void updateClockScreen();
void updateCurrentWeatherScreen();
void updateForecastWeatherScreen();
char *getUpdatedDate();
char *getDisplayTime();
void setConnectiontToNTPScreen();
void dealWithUpdates();
void halfHourUpdate();
void setMeteoClockInterface();
void dealWithFailedRequests();
void midnightUpdate();