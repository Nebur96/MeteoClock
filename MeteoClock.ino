#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>

#include "GfxUi.h"
#include "Constants.h"
#include "Images.h"
#include "Weather.h"

Weather weather = Weather(WUNDERGROUND_API, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY, numberOfForecastPredictions);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
GfxUi ui = GfxUi(&tft);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

int hours = 0;
int minutes = 0;
int seconds = 0;
int lastMillis = 0;

int elapsedMinutes = 0;
int elapsedHours = 0;

void setup(void)
{
  Serial.begin(115200);

  WiFi.hostname(HOSTNAME);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  setInitializationScreen();
  setConnectionToWifiScreen();
  setDownloadScreen();
  setMeteoClockInterface();
  
  elapsedHours = hours;
  elapsedMinutes = (minutes < 30) ? 30 - minutes : minutes - 30;
}

void loop()
{
  if (millis() - lastMillis >= 1000)
  {
    lastMillis = millis();
    seconds++;
    if (seconds >= 60)
    {
      minutes++;
      elapsedMinutes++;
      seconds = 0;

      if (minutes >= 60)
      {
        minutes = 0;
        hours++;
        elapsedHours++;

        if (hours >= 24)
        {
          hours = 0;
        }
      }

      updateClockScreen();
    }

    if (elapsedHours == 24) {
      getUpdatedHours();
      weather.getCurrentWeatherConditions();
      weather.getWeatherForecast();
      setMeteoClockInterface();
    } else if (elapsedMinutes == 30) {
      getUpdatedHours();
      weather.getCurrentWeatherConditions();
      updateCurrentWeatherScreen();
      elapsedMinutes = 0;
    }

  }
}

void getUpdatedHours() {
  timeClient.update();

  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  seconds = timeClient.getSeconds();
}

void setInitializationScreen()
{
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setTextSize(3);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 140, "MeteoClock");
}

void setConnectionToWifiScreen()
{
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 130, "Connected to: \n");
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.drawString(120, 170, String(ssid));
}

void setDownloadScreen()
{
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 130, "Loading info...");

  ui.drawProgressBar(0, 170, 240, 30, 0, ILI9341_WHITE, ILI9341_DARKCYAN);
  timeClient.begin();
  ui.drawProgressBar(0, 170, 240, 30, 25, ILI9341_WHITE, ILI9341_DARKCYAN);
  getUpdatedHours();
  ui.drawProgressBar(0, 170, 240, 30, 50, ILI9341_WHITE, ILI9341_DARKCYAN);
  weather.getCurrentWeatherConditions();
  ui.drawProgressBar(0, 170, 240, 30, 75, ILI9341_WHITE, ILI9341_DARKCYAN);
  weather.getWeatherForecast();
  ui.drawProgressBar(0, 170, 240, 30, 100, ILI9341_WHITE, ILI9341_DARKCYAN);
}

void setMeteoClockInterface()
{
  tft.fillScreen(ILI9341_BLACK);
  yield();

  updateDateScreen();

  updateClockScreen();

  updateCurrentWeatherScreen();

  updateForecastWeatherScreen();
}

void updateDateScreen() {
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  tft.setTextSize(2);
  ui.drawString(120, 10, getUpdatedDate());
}

void updateClockScreen()
{
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  tft.setTextSize(8);
  ui.drawString(125, 40, getDisplayTime());
}

void updateCurrentWeatherScreen() {
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(3);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 120, weather.conditions.weather);
  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_RED, ILI9341_BLACK);
  ui.drawString(120, 160, String(weather.forecasts[0].high_temperature));
  ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  ui.drawString(180, 160, String(weather.forecasts[0].low_temperature));
  tft.setTextSize(4);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.drawString(100, 190, String(weather.conditions.temperature) + String((char)247) + "C");

  tft.drawRGBBitmap(10, 150, getCurrentWeatherBigImage(parseIconsFromWebsite(weather.conditions.icon)), 80, 80);
}

void updateForecastWeatherScreen() {
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);

  ui.setTextAlignment(CENTER);
  ui.drawString(60, 250, weather.forecasts[1].weekDay);
  tft.drawRGBBitmap(20, 270, getCurrentWeatherSmallImage(weather.forecasts[1].icon), 50, 50);

  ui.drawStringInInterval(130, 240, 250, weather.forecasts[2].weekDay);
  tft.drawRGBBitmap(140, 270, getCurrentWeatherSmallImage(weather.forecasts[2].icon), 50, 50);

  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_RED, ILI9341_BLACK);
  ui.drawString(80, 275, weather.forecasts[1].high_temperature);
  ui.drawString(200, 275, weather.forecasts[2].high_temperature);

  ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  ui.drawString(80, 295, weather.forecasts[1].low_temperature);
  ui.drawString(200, 295, weather.forecasts[2].low_temperature);
}

String getUpdatedDate()
{
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  time_t local, utc;
  utc = epochTime;
  TimeChangeRule usEDT =
  {
    "EDT", Second, Sun, Mar, 2, -300
  };
  TimeChangeRule usEST =
  {
    "EST", First, Sun, Nov, 2, -360
  };
  Timezone usEastern(usEDT, usEST);
  local = usEastern.toLocal(utc);

  return daysOfWeek[weekday(local) - 1] + String(", ") + day(local) + String(" ") + months[month(local) - 1] + String(" ") + year(local);
}

String getDisplayTime()
{
  String minutesToDisplay = (minutes >= 10) ? String(minutes) : "0" + String(minutes);
  String hoursToDisplay = (hours >= 10) ? String(hours) : "0" + String(hours);
  return hoursToDisplay + ":" + minutesToDisplay;
}

String parseIconsFromWebsite(String currentIcon){
  return (hours <= nightUntil || hours >= nightFrom) ? "nt_" + currentIcon : currentIcon;
}

const unsigned short* getCurrentWeatherBigImage(String currentIcon) {
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

const unsigned short* getCurrentWeatherSmallImage(String currentIcon) {
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
