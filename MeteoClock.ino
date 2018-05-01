#include "MeteoClock.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
GfxUi ui = GfxUi(&tft);
Image image = Image();
Weather weather = Weather(WUNDERGROUND_API, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY, NUMBER_OF_FORECAST_PREDICTIONS);
Astronomy astronomy = Astronomy(LAT, LNG, SUNRISE_HOURS, SUNSET_HOURS);

Errors error;

unsigned long lastMinute = 999999;

TimerObject *midnightUpdateTimer = new TimerObject(10000, &midnightUpdate);

/********** CALLBACKS **********/

void configModeCallback (WiFiManager *myWiFiManager) {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  tft.drawRGBBitmap(80, 70, image.getNoWiFi(), image.BIG_IMAGE_WIDTH, image.BIG_IMAGE_HEIGH);
  ui.drawString(120, 170, "No connection,");
  ui.drawString(120, 200, "configure a new AP!");
}

void time_is_set (void)
{
  gettimeofday(&cbtime, nullptr);
  now = time(nullptr);

  tm* tm = localtime(&now);
  cbtime_set = true;

  defineCurrentDayTime(tm);
}

/*******************************/

void setup(void)
{
  setInitializationScreen();

  Serial.begin(115200);

  error.sunPositionsError = false;
  error.currentConditionsError = false;
  error.forecastConditionsError = false;

  //reset saved settings
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setTimeout(180);
  if (!wifiManager.autoConnect(HOSTNAME)) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  setDownloadScreen();
  setConnectiontToNTPScreen();

  settimeofday_cb(time_is_set);
  configTime(TZ_SEC, DST_SEC, NTP_POOL);

  while (!cbtime_set) {
    delay(100);
  }

  setMeteoClockInterface();

  delay(2000);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    return;
  }

  gettimeofday(&cbtime, nullptr);
  now = time(nullptr);

  tm* tm = localtime(&now);

  midnightUpdateTimer->Update();
  dealWithFailedRequests(tm);

  int currentMinute = minute(tm);
  if (lastMinute != currentMinute) {
    lastMinute = currentMinute;
    updateClockScreen();
    dealWithUpdates(tm);
  }
}

void dealWithUpdates(const tm* tm) {
  int minutes = minute(tm);
  if (hour(tm) == 0 && minutes == 0) {
    midnightUpdateTimer->Start();
  } else if (minutes % 30 == 0) {
    halfHourUpdate(tm);
  }
}

void dealWithFailedRequests(const tm* tm) {
  if (error.sunPositionsError) {
    error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
    defineCurrentDayTime(tm);
  } else if (error.currentConditionsError) {
    error.currentConditionsError = weather.getCurrentWeatherConditions();
  } else if (error.forecastConditionsError) {
    error.forecastConditionsError = weather.getWeatherForecast();
  }
}

void midnightUpdate() {
  midnightUpdateTimer->Stop();
  error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  error.forecastConditionsError = weather.getWeatherForecast();
  setMeteoClockInterface();
}

void halfHourUpdate(const tm* tm) {
  defineCurrentDayTime(tm);
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  updateCurrentWeatherScreen();
}

void setInitializationScreen()
{
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(3);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 140, HOSTNAME);
  delay(2000);
}

void setDownloadScreen()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 130, "Loading info...");
  ui.drawProgressBar(0, 170, 238, 30, 0, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
  ui.drawProgressBar(0, 170, 238, 30, 33, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  ui.drawProgressBar(0, 170, 238, 30, 66, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.forecastConditionsError = weather.getWeatherForecast();
  ui.drawProgressBar(0, 170, 238, 30, 100, ILI9341_WHITE, ILI9341_DARKCYAN);
}

void setConnectiontToNTPScreen()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 130, "Waiting NTP server");
  ui.drawString(120, 150, "response");
}

void setMeteoClockInterface()
{
  tft.fillScreen(ILI9341_BLACK);
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
  tft.setTextSize(2);

  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_RED, ILI9341_BLACK);
  ui.drawString(80, 275, weather.forecasts[1].high_temperature);
  ui.drawString(200, 275, weather.forecasts[2].high_temperature);

  ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  ui.drawString(80, 295, weather.forecasts[1].low_temperature);
  ui.drawString(200, 295, weather.forecasts[2].low_temperature);

  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(60, 250, weather.forecasts[1].weekDay);
  tft.drawRGBBitmap(20, 270, image.getCurrentWeatherSmallImage(weather.forecasts[1].icon), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);

  ui.drawStringInInterval(130, 240, 250, weather.forecasts[2].weekDay);
  tft.drawRGBBitmap(140, 270, image.getCurrentWeatherSmallImage(weather.forecasts[2].icon), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);
}

void defineCurrentDayTime(const tm* tm) {
  int hours = hour(tm);
  image.setCurrentDayTime((hours <= astronomy.sunriseHour || hours >= astronomy.sunsetHour) ? NIGHT : DAY);
}

String getDisplayTime()
{
  const tm* tm = localtime(&now);
  int minutes = minute(tm);
  int hours = hour(tm);
  String minutesToDisplay = (minutes >= 10) ? String(minutes) : "0" + String(minutes);
  String hoursToDisplay = (hours >= 10) ? String(hours) : "0" + String(hours);
  return hoursToDisplay + ":" + minutesToDisplay;
}

String getUpdatedDate()
{
  const tm* tm = localtime(&now);
  return daysOfWeek[weekday(tm)] + String(", ") + day(tm) + String(" ") + months[month(tm) - 1] + String(" ") + (BASE_YEAR + year(tm));
}

int hour(const tm* tm) {
  return tm->tm_hour;
}

int minute(const tm* tm) {
  return tm->tm_min;
}

int day(const tm* tm) {
  return tm->tm_mday;
}

int month(const tm* tm) {
  return tm->tm_mon;
}

int year(const tm* tm) {
  return tm->tm_year;
}

int weekday(const tm* tm) {
  return tm->tm_wday;
}
