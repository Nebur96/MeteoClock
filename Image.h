#include<WString.h>

#ifndef Image_h
#define Image_h

enum DayTime {
  DAY, NIGHT
};

class Image
{
  public:
    Image();
    const unsigned short* getCurrentWeatherBigImage(String currentIcon);
    const unsigned short* getCurrentWeatherSmallImage(String currentIcon);
    const unsigned short* getNoWiFi();
    void setCurrentDayTime(DayTime dayTime);
    const int BIG_IMAGE_HEIGH = 80;
    const int  BIG_IMAGE_WIDTH = 80;
    const int  SMALL_IMAGE_HEIGH = 50;
    const int  SMALL_IMAGE_WIDTH = 50;
  private:
    DayTime _dayTime = DAY;
};

#endif
