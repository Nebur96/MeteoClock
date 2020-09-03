#include<WString.h>

#ifndef Image_h
#define Image_h

enum ImageSize {
  BIG, SMALL
};

class Image
{
  public:
    Image();
    const unsigned short* getCurrentWeatherImage(String icon, ImageSize size);
    const unsigned short* getNoWiFi();
    const int BIG_IMAGE_HEIGH = 80;
    const int  BIG_IMAGE_WIDTH = 80;
    const int  SMALL_IMAGE_HEIGH = 50;
    const int  SMALL_IMAGE_WIDTH = 50;
  private:
    const unsigned short* getCurrentWeatherBigImage(String icon);
    const unsigned short* getCurrentWeatherSmallImage(String icon);
};

#endif
