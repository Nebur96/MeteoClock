//SCREEN LIBRARIES AND GRAPHICS
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "GfxUi.h"
#include "Image.h"

//JSON LIBRARY
#include <ArduinoJson.h>         //https://github.com/bblanchon/ArduinoJson

//WIFI CONNECTION LIBRARIES
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

//TIME LIBRARIES
#include <time.h>
#include <sys/time.h>
#include <coredecls.h>

//TIMER LIBRARIES
#include <TimerObject.h>         //https://github.com/aron-bordin/ArduinoTimerObject

//WEATHER LIBRARY
#include "Weather.h"

//ASTRONOMY LIBRARY
#include "Astronomy.h"

//PROJECT CONSTANTS AND EXTERNAL VARIABLES
#include "Configs.h"

#define HOSTNAME "MeteoClock"

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

//TIME CONSTS
#define BASE_YEAR 1900

//MONTHS NAMES
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

//DAYS OF WEEK NAMES
const char * daysOfWeek[] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};

//VARIABLE DECLARATIONS
timeval cbtime;
bool cbtime_set = false;
time_t now;

WiFiManager wifiManager;

typedef struct {
  bool sunPositionsError;
  bool currentConditionsError;
  bool forecastConditionsError;
} Errors;






