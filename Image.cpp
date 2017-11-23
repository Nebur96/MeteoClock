#include "Image.h"
#include "Images_Store.h"

Image::Image() {
};

void Image::setCurrentDayTime(DayTime dayTime) {
  _dayTime = dayTime;
}

const unsigned short* Image::getNoWiFi() {
  return noWiFi;
}

const unsigned short* Image::getCurrentWeatherBigImage(String currentIcon) {
  currentIcon = (_dayTime == DAY) ? currentIcon : "nt_" + currentIcon;

  if (currentIcon == "sunny") {
    return sunny_big;
  } else if (currentIcon == "clear") {
    return sunny_big;
  } else if (currentIcon == "clear") {
    return sunny_big;
  } else if (currentIcon == "chanceflurries") {
    return snow_big;
  } else if (currentIcon == "chancesnow") {
    return snow_big;
  } else if (currentIcon == "flurries") {
    return snow_big;
  } else if (currentIcon == "nt_chancesnow") {
    return snow_big;
  } else if (currentIcon == "nt_chanceflurries") {
    return snow_big;
  } else if (currentIcon == "nt_flurries") {
    return snow_big;
  } else if (currentIcon == "nt_snow") {
    return snow_big;
  } else if (currentIcon == "snow") {
    return snow_big;
  } else if (currentIcon == "chancerain") {
    return rain_big;
  } else if (currentIcon == "nt_chancerain") {
    return rain_big;
  } else if (currentIcon == "nt_rain") {
    return rain_big;
  } else if (currentIcon == "rain") {
    return rain_big;
  } else if (currentIcon == "chancesleet") {
    return rain_big;
  } else if (currentIcon == "nt_chancesleet") {
    return rain_big;
  } else if (currentIcon == "nt_sleet") {
    return rain_big;
  } else if (currentIcon == "sleet") {
    return rain_big;
  } else if (currentIcon == "chancetstorms") {
    return lightning_big;
  } else if (currentIcon == "nt_tstorms") {
    return lightning_big;
  } else if (currentIcon == "chancetstorms") {
    return lightning_big;
  } else if (currentIcon == "nt_chancetstorms") {
    return lightning_big;
  } else if (currentIcon == "tstorms") {
    return lightning_big;
  } else if (currentIcon == "cloudy") {
    return cloudy_big;
  } else if (currentIcon == "nt_cloudy") {
    return cloudy_big;
  } else if (currentIcon == "nt_fog") {
    return cloudy_big;
  } else if (currentIcon == "fog") {
    return cloudy_big;
  } else if (currentIcon == "nt_hazy") {
    return cloudy_big;
  } else if (currentIcon == "hazy") {
    return cloudy_big;
  } else if (currentIcon == "nt_clear") {
    return moon_big;
  } else if (currentIcon == "nt_sunny") {
    return moon_big;
  } else if (currentIcon == "mostlycloudy") {
    return sun_most_cloudy_big;
  } else if (currentIcon == "mostlysunny") {
    return sun_and_clouds_big;
  } else if (currentIcon == "partlycloudy") {
    return sun_and_clouds_big;
  } else if (currentIcon == "partlysunny") {
    return sun_most_cloudy_big;
  } else if (currentIcon == "nt_mostlycloudy") {
    return moon_most_cloudy_big;
  } else if (currentIcon == "nt_mostlysunny") {
    return moon_cloudy_big;
  } else if (currentIcon == "nt_partlycloudy") {
    return moon_cloudy_big;
  } else if (currentIcon == "nt_partlysunny") {
    return moon_most_cloudy_big;
  } else {
    return unknown_big;
  }
}

const unsigned short* Image::getCurrentWeatherSmallImage(String currentIcon) {

  if (currentIcon == "sunny") {
    return sunny_small;
  } else if (currentIcon == "clear") {
    return sunny_small;
  } else if (currentIcon == "clear") {
    return sunny_small;
  } else if (currentIcon == "chanceflurries") {
    return snow_small;
  } else if (currentIcon == "chancesnow") {
    return snow_small;
  } else if (currentIcon == "flurries") {
    return snow_small;
  } else if (currentIcon == "nt_chancesnow") {
    return snow_small;
  } else if (currentIcon == "nt_chanceflurries") {
    return snow_small;
  } else if (currentIcon == "nt_flurries") {
    return snow_small;
  } else if (currentIcon == "nt_snow") {
    return snow_small;
  } else if (currentIcon == "snow") {
    return snow_small;
  } else if (currentIcon == "chancerain") {
    return rain_small;
  } else if (currentIcon == "nt_chancerain") {
    return rain_small;
  } else if (currentIcon == "nt_rain") {
    return rain_small;
  } else if (currentIcon == "rain") {
    return rain_small;
  } else if (currentIcon == "chancesleet") {
    return rain_small;
  } else if (currentIcon == "nt_chancesleet") {
    return rain_small;
  } else if (currentIcon == "nt_sleet") {
    return rain_small;
  } else if (currentIcon == "sleet") {
    return rain_small;
  } else if (currentIcon == "chancetstorms") {
    return lightning_small;
  } else if (currentIcon == "nt_tstorms") {
    return lightning_small;
  } else if (currentIcon == "chancetstorms") {
    return lightning_small;
  } else if (currentIcon == "nt_chancetstorms") {
    return lightning_small;
  } else if (currentIcon == "tstorms") {
    return lightning_small;
  } else if (currentIcon == "cloudy") {
    return cloudy_small;
  } else if (currentIcon == "nt_cloudy") {
    return cloudy_small;
  } else if (currentIcon == "nt_fog") {
    return cloudy_small;
  } else if (currentIcon == "fog") {
    return cloudy_small;
  } else if (currentIcon == "nt_hazy") {
    return cloudy_small;
  } else if (currentIcon == "hazy") {
    return cloudy_small;
  } else if (currentIcon == "nt_clear") {
    return moon_small;
  } else if (currentIcon == "nt_sunny") {
    return moon_small;
  } else if (currentIcon == "mostlycloudy") {
    return sun_most_cloudy_small;
  } else if (currentIcon == "mostlysunny") {
    return sun_and_clouds_small;
  } else if (currentIcon == "partlycloudy") {
    return sun_and_clouds_small;
  } else if (currentIcon == "partlysunny") {
    return sun_most_cloudy_small;
  } else if (currentIcon == "nt_mostlycloudy") {
    return moon_most_cloudy_small;
  } else if (currentIcon == "nt_mostlysunny") {
    return moon_cloudy_small;
  } else if (currentIcon == "nt_partlycloudy") {
    return moon_cloudy_small;
  } else if (currentIcon == "nt_partlysunny") {
    return moon_most_cloudy_small;
  } else {
    return unknown_small;
  }
}
