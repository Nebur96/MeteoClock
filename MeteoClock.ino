#include "MeteoClock.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
GfxUi ui = GfxUi(&tft);
Image image = Image();
Weather weather = Weather(WUNDERGROUND_API, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY, NUMBER_OF_FORECAST_PREDICTIONS);
Astronomy astronomy = Astronomy(LAT, LNG, SUNRISE_HOURS, SUNSET_HOURS);
DaylightSaving daylightSaving = DaylightSaving();

WiFiUDP ntpUDP;

Errors error;

unsigned long lastMinute = 999999;

byte packetBuffer[NTP_PACKET_SIZE];

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
  //INTERFACE DRAWING
  setDownloadScreen();
  setMeteoClockInterface();
}

void configModeCallback (WiFiManager *myWiFiManager) {
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setTextSize(2);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  tft.drawRGBBitmap(80, 70, image.getNoWiFi(), image.BIG_IMAGE_WIDTH, image.BIG_IMAGE_HEIGH);
  ui.drawString(120, 170, "No connection,");
  ui.drawString(120, 200, "configure a new AP!");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    return;
  }

  dealWithFailedRequests();

  if (lastMinute != minute())
  {
    lastMinute = minute();
    updateClockScreen();
    dealWithUpdates();
  }
}

void dealWithUpdates() {
  if (hour() == 0 && minute() == 0) {
    midnightUpdate();
  } else if (minute() % 30 == 0) {
    halfHourUpdate();
  }
}

void dealWithFailedRequests() {
  if (error.sunPositionsError) {
    error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
    defineCurrentDayTime();
  } else if (error.currentConditionsError) {
    error.currentConditionsError = weather.getCurrentWeatherConditions();
  } else if (error.forecastConditionsError) {
    error.forecastConditionsError = weather.getWeatherForecast();
  }
}

void midnightUpdate() {
  error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  error.forecastConditionsError = weather.getWeatherForecast();
  setMeteoClockInterface();
}

void halfHourUpdate() {
  defineCurrentDayTime();
  daylightSaving.calculateDaySavingTime(hour(), day(), month(), year());
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  updateCurrentWeatherScreen();
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
  ui.drawString(120, 140, HOSTNAME);
  delay(2000);
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
  ntpUDP.begin(localPort);
  ui.drawProgressBar(0, 170, 238, 30, 20, ILI9341_WHITE, ILI9341_DARKCYAN);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  ui.drawProgressBar(0, 170, 238, 30, 40, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.sunPositionsError = astronomy.updateSunriseAndSunsetHours();
  daylightSaving.calculateDaySavingTime(hour(), day(), month(), year());
  defineCurrentDayTime();
  ui.drawProgressBar(0, 170, 238, 30, 60, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.currentConditionsError = weather.getCurrentWeatherConditions();
  ui.drawProgressBar(0, 170, 238, 30, 80, ILI9341_WHITE, ILI9341_DARKCYAN);
  error.forecastConditionsError = weather.getWeatherForecast();
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

void defineCurrentDayTime() {
  int hours = hour();
  image.setCurrentDayTime((hours <= astronomy.sunriseHour || hours >= astronomy.sunsetHour) ? NIGHT : DAY);
}

String getDisplayTime()
{
  String minutesToDisplay = (minute() >= 10) ? String(minute()) : "0" + String(minute());
  String hoursToDisplay = (hour() >= 10) ? String(hour()) : "0" + String(hour());
  return hoursToDisplay + ":" + minutesToDisplay;
}

String getUpdatedDate()
{
  return daysOfWeek[(weekday() - 1)] + String(", ") + day() + String(" ") + months[month() - 1] + String(" ") + year();
}

void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntpUDP.beginPacket(address, 123); //NTP requests are to port 123
  ntpUDP.write(packetBuffer, NTP_PACKET_SIZE);
  ntpUDP.endPacket();
}

time_t getNtpTime()
{
  IPAddress ntpServerIP;
  while (ntpUDP.parsePacket() > 0);
  WiFi.hostByName(NTP_POOL, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 20000) {
    int size = ntpUDP.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      ntpUDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + daylightSaving.timeZone * SECS_PER_HOUR;
    }
  }

  return 0;
}
