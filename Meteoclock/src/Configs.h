//WEATHER PARAMETERS
#define OPEN_WEATHER_KEY "************************"

//GPS COORDINATES
const char *LAT = "********";
const char *LNG = "********";

//DEFAULT SUNSET & SUNRISE HOURS
const int SUNSET_HOURS = 17;
const int SUNRISE_HOURS = 7;

//NTP CLIENT PARAMETERS
const char *NTP_POOL = "pool.ntp.org";

#define TZ 0      // (utc+) TZ in hours
#define DST_MN 60 // use 60mn for summer time in some countries
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)

#define BAULD_RATE 115200