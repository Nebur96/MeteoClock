#include <Wire.h>
#include <limits.h>

#include "MeteoClock.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
GfxUi ui = GfxUi(&tft);
Image image = Image();
Weather weather = Weather(OPEN_WEATHER_KEY, LAT, LNG);
MTime mtime = MTime(TZ_SEC, DST_SEC, NTP_POOL);

Errors error;

unsigned int lastMinute = UINT_MAX;

TimerObject *midnightUpdateTimer = new TimerObject(10000, &midnightUpdate);

/********** CALLBACKS **********/

void configModeCallback(WiFiManager *myWiFiManager)
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextSize(2);
    ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    ui.setTextAlignment(CENTER);
    tft.drawRGBBitmap(80, 70, image.getNoWiFi(), image.BIG_IMAGE_WIDTH, image.BIG_IMAGE_HEIGH);
    ui.drawString(120, 170, NO_CONNECTION_MESSAGE_1);
    ui.drawString(120, 200, NO_CONNECTION_MESSAGE_2);
}

/*******************************/

void setup(void)
{
    setInitializationScreen();

    Serial.begin(115200);

    error.currentConditionsError = false;
    error.forecastConditionsError = false;

    //reset saved settings
    //wifiManager.resetSettings();
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setTimeout(180);
    if (!wifiManager.autoConnect(HOSTNAME))
    {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        ESP.reset();
        delay(5000);
    }

    setDownloadScreen();
    setConnectiontToNTPScreen();
    mtime.initializeTime();
    setMeteoClockInterface();

    Serial.println(ESP.getFreeHeap(), DEC);

    delay(2000);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        return;
    }

    mtime.loop();

    midnightUpdateTimer->Update();
    dealWithFailedRequests();

    unsigned int currentMinute = mtime.minute();
    if (lastMinute != currentMinute)
    {
        lastMinute = currentMinute;
        updateClockScreen();
        dealWithUpdates();
    }
}

void dealWithUpdates()
{
    unsigned int minutes = mtime.minute();
    if (mtime.hour() == 0 && minutes == 0)
    {
        midnightUpdateTimer->Start();
    }
    else if (minutes % 30 == 0)
    {
        halfHourUpdate();
    }
}

void dealWithFailedRequests()
{
    if (error.currentConditionsError)
    {
        error.currentConditionsError = weather.getCurrentWeatherConditions();
    }
    else if (error.forecastConditionsError)
    {
        error.forecastConditionsError = weather.getWeatherForecast();
    }
}

void midnightUpdate()
{
    midnightUpdateTimer->Stop();
    error.currentConditionsError = weather.getCurrentWeatherConditions();
    error.forecastConditionsError = weather.getWeatherForecast();
    setMeteoClockInterface();
}

void halfHourUpdate()
{
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
    ui.drawString(120, 130, LOADING_MESSAGE);
    ui.drawProgressBar(0, 170, 238, 30, 0, ILI9341_WHITE, ILI9341_DARKCYAN);
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
    ui.drawString(120, 130, CONNECTION_TO_NTP_SERVER_MESSAGE_1);
    ui.drawString(120, 150, CONNECTION_TO_NTP_SERVER_MESSAGE_2);
}

void setMeteoClockInterface()
{
    tft.fillScreen(ILI9341_BLACK);
    updateDateScreen();
    updateClockScreen();
    updateCurrentWeatherScreen();
    updateForecastWeatherScreen();
}

void updateDateScreen()
{
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

void updateCurrentWeatherScreen()
{
    ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(3);
    ui.setTextAlignment(CENTER);
    ui.drawString(120, 120, weather.conditions.weather);
    ui.setTextAlignment(LEFT);
    ui.setTextColor(ILI9341_RED, ILI9341_BLACK);
    ui.drawString(120, 160, weather.forecasts[0].high_temperature);
    ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
    ui.drawString(180, 160, weather.forecasts[0].low_temperature);
    tft.setTextSize(4);
    ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    ui.drawString(100, 190, weather.conditions.temperature + String((char)247) + "C");

    tft.drawRGBBitmap(10, 150, image.getCurrentWeatherImage(weather.conditions.icon, BIG), image.BIG_IMAGE_WIDTH, image.BIG_IMAGE_HEIGH);
}

void updateForecastWeatherScreen()
{
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
    tft.drawRGBBitmap(20, 270, image.getCurrentWeatherImage(weather.forecasts[1].icon, SMALL), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);

    ui.drawStringInInterval(130, 240, 250, weather.forecasts[2].weekDay);
    tft.drawRGBBitmap(140, 270, image.getCurrentWeatherImage(weather.forecasts[2].icon, SMALL), image.SMALL_IMAGE_WIDTH, image.SMALL_IMAGE_HEIGH);
}

char *getDisplayTime()
{
    int minutes = mtime.minute();
    int hours = mtime.hour();

    char *hoursToDisplay = (char *)malloc(HOUR_SIZE);
    sprintf(hoursToDisplay, "%02d:%02d", hours, minutes);

    return hoursToDisplay;
}

char *getUpdatedDate()
{
    char *date = (char *)malloc(DATE_SIZE);
    sprintf(date, "%s, %d %s %d", mtime.weekdayName(), mtime.day(), mtime.monthName(), mtime.year());
    return date;
}