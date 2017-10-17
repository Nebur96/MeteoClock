#define HOSTNAME "MeteoClock"

#define BIG_IMAGE_SIZE 80
#define BIG_IMAGE_WIDTH 80
#define SMALL_IMAGE_HEIGH 50
#define SMALL_IMAGE_WIDTH 50

// LCD Pins Connections
#define TFT_CS D2
#define TFT_DC D1
#define TFT_MOSI D7
#define TFT_CLK D5
#define TFT_RST D4
#define TFT_MISO D6

//WUNDERGROUND PARAMETERS
#define WUNDERGROUND_API "<API_KEY_WUNDERGROUND>"
#define WUNDERGROUND_COUNTRY "<COUNTRY_WUNDERGROUND>"
#define WUNDERGROUND_CITY "<CITY_WUNDERGROUND>"

const int numberOfForecastPredictions  = 3;
const int nightFrom = 19;
const int nightUntil = 8;

const char * ssid = "<SSID>";
const char * password = "<PASSWORD>";

const char * months[] =
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"
};

const char * daysOfWeek[] =
{
  "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"
};
