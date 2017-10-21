#define HOSTNAME "MeteoClock"

// LCD PINS CONNECTIONS
#define TFT_CS D2
#define TFT_DC D1
#define TFT_MOSI D7
#define TFT_CLK D5
#define TFT_RST D4
#define TFT_MISO D6

//WEATHER PARAMETERS
#define WUNDERGROUND_API "<API_KEY_WUNDERGROUND>"
#define WUNDERGROUND_COUNTRY "<COUNTRY_WUNDERGROUND>"
#define WUNDERGROUND_CITY "<CITY_WUNDERGROUND>"
#define NUMBER_OF_FORECAST_PREDICTIONS 3

//WiFi AP SETTINGS
const char * ssid = "<SSID>";
const char * password = "<PASSWORD>";

//GPS COORDINATES
const char* lat = "<LATITUDE>";
const char* lng = "<LONGITUDE>";


//MONTHS NAMES
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

//DAYS OF WEEK NAMES
const char * daysOfWeek[] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};

//HOURS CONVERSION AFTER 12 AM FOR 24 HOURS CLOCK
const int hoursPM[] = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 12};
