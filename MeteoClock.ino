//SCREEN LIBRARIES AND GRAPHICS
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "GfxUi.h"
#include "Image.h"

//JSON LIBRARY
#include <ArduinoJson.h>

//WIFI CONNECTION LIBRARIES
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//TIME LIBRARIES
#include <NTPClient.h>
#include <Timezone.h>
#include <TimeLib.h>

//WEATHER LIBRARY
#include "Weather.h"

//PROJECT CONSTANTS VARIABLES
#include "Constants.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
GfxUi ui = GfxUi(&tft);
Image image = Image();
Weather weather = Weather(WUNDERGROUND_API, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY, NUMBER_OF_FORECAST_PREDICTIONS);

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

typedef struct {
  int hours;
  int minutes;
  int seconds;
} Time;

Time t;

int elapsedMinutes = 0;
int elapsedHours = 0;

int sunriseHour = 7;
int sunsetHour = 19;

unsigned long lastMillis = 0;

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

  //INTERFACE DRAWING
  setInitializationScreen();
  setConnectionToWifiScreen();
  setDownloadScreen();
  setMeteoClockInterface();

  elapsedHours = t.hours;
  elapsedMinutes = (t.minutes < 30) ? 30 - t.minutes : t.minutes - 30;
}

void loop()
{
  if (millis() - lastMillis >= 1000)
  {
    lastMillis = millis();
    t.seconds++;
    if (t.seconds >= 60)
    {
      t.minutes++;
      elapsedMinutes++;
      t.seconds = 0;

      if (t.minutes >= 60)
      {
        t.minutes = 0;
        t.hours++;
        elapsedHours++;

        if (t.hours >= 24)
        {
          t.hours = 0;
        }
      }
      updateClockScreen();
    }

    if (elapsedHours == 24) {
      getUpdatedHours();
      weather.getCurrentWeatherConditions();
      weather.getWeatherForecast();
      setMeteoClockInterface();
      elapsedHours = 0;
      elapsedMinutes = 0;
    } else if (elapsedMinutes == 30) {
      getUpdatedHours();
      defineCurrentDayTime();
      weather.getCurrentWeatherConditions();
      updateCurrentWeatherScreen();
      elapsedMinutes = 0;
    }
  }
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

  ui.drawProgressBar(0, 170, 238, 30, 0, ILI9341_WHITE, ILI9341_DARKCYAN);
  timeClient.begin();
  ui.drawProgressBar(0, 170, 238, 30, 20, ILI9341_WHITE, ILI9341_DARKCYAN);
  getUpdatedHours();
  ui.drawProgressBar(0, 170, 238, 30, 40, ILI9341_WHITE, ILI9341_DARKCYAN);
  getSunriseAndSunsetHours();
  defineCurrentDayTime();
  ui.drawProgressBar(0, 170, 238, 30, 60, ILI9341_WHITE, ILI9341_DARKCYAN);
  weather.getCurrentWeatherConditions();
  ui.drawProgressBar(0, 170, 238, 30, 80, ILI9341_WHITE, ILI9341_DARKCYAN);
  weather.getWeatherForecast();
  ui.drawProgressBar(0, 170, 238, 30, 100, ILI9341_WHITE, ILI9341_DARKCYAN);
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

  tft.drawRGBBitmap(10, 150, image.getCurrentWeatherBigImage(weather.conditions.icon), image.BIG_IMAGE_WIDTH, image.BIG_IMAGE_HEIGH);
}

void updateForecastWeatherScreen() {
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);

  ui.setTextAlignment(CENTER);
  ui.drawString(60, 250, weather.forecasts[1].weekDay);
  tft.drawRGBBitmap(20, 270, image.getCurrentWeatherSmallImage(weather.forecasts[1].icon), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);

  ui.drawStringInInterval(130, 240, 250, weather.forecasts[2].weekDay);
  tft.drawRGBBitmap(140, 270, image.getCurrentWeatherSmallImage(weather.forecasts[2].icon), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);

  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_RED, ILI9341_BLACK);
  ui.drawString(80, 275, weather.forecasts[1].high_temperature);
  ui.drawString(200, 275, weather.forecasts[2].high_temperature);

  ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  ui.drawString(80, 295, weather.forecasts[1].low_temperature);
  ui.drawString(200, 295, weather.forecasts[2].low_temperature);
}

String defineCurrentDayTime() {
  image.setCurrentDayTime((t.hours <= sunriseHour || t.hours >= sunsetHour) ? NIGHT : DAY);
}

void getUpdatedHours() {
  timeClient.update();
  
  t.hours = timeClient.getHours();
  t.minutes = timeClient.getMinutes();
  t.seconds = timeClient.getSeconds();
}

String getDisplayTime()
{
  String minutesToDisplay = (t.minutes >= 10) ? String(t.minutes) : "0" + String(t.minutes);
  String hoursToDisplay = (t.hours >= 10) ? String(t.hours) : "0" + String(t.hours);
  return hoursToDisplay + ":" + minutesToDisplay;
}

String getUpdatedDate()
{
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  time_t local, utc;
  utc = epochTime;
  TimeChangeRule WET =
  {
    "WET", Last, Sun, Mar, 1, -60
  };
  TimeChangeRule WEST =
  {
    "WEST", Last, Sun, Oct, 2, 0
  };
  
  Timezone EUR(WET, WEST);
  local = EUR.toLocal(utc);

  return daysOfWeek[weekday(local) - 1] + String(", ") + day(local) + String(" ") + months[month(local) - 1] + String(" ") + year(local);
}


void getSunriseAndSunsetHours()
{
  WiFiClient client;

  String response = "";

  const char* HOST = "api.sunrise-sunset.org";
  const int HTTP_PORT = 80;

  if (!client.connect(HOST, HTTP_PORT))
  {
    Serial.println("connection failed");
  }

  String url = "/json?lat=";
  url += lat;
  url += "&lng=";
  url += lng;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client Timeout Conditions!");
      client.stop();
      return;
    }
  }

  while (client.available()) {
    response += client.readStringUntil('\r');
  }

  response = response.substring(response.indexOf("{"));

  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(response);
  jsonBuffer.clear();

  if (root["status"] == "OK") {
    String result = root["results"]["sunrise"].as<String>();
    sunriseHour = result.substring(0, 1).toInt();

    result = root["results"]["sunset"].as<String>();
    sunsetHour = hoursPM[result.substring(0, 1).toInt() - 1];
  } else {
    sunriseHour = 7;
    sunsetHour = 19;
  }
}
