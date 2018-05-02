//WEATHER PARAMETERS
#define WUNDERGROUND_API "<WUNDERGROUD_API>"
#define WUNDERGROUND_COUNTRY "<COUNTRY>"
#define WUNDERGROUND_CITY "<CITY>"

//GPS COORDINATES
const char* LAT = "<LATITUDE>";
const char* LNG = "<LONGITUDE>";

//DEFAULT SUNSET & SUNRISE HOURS
const int SUNSET_HOURS = 17;
const int SUNRISE_HOURS = 7;

//NTP CLIENT PARAMETERS
const char* NTP_POOL = "pool.ntp.org";
//
#define TZ              0           // (utc+) TZ in hours
#define DST_MN          60          // use 60mn for summer time in some countries
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)
