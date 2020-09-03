#include "Image.h"
#include "Images_Store.h"

Image::Image() {};

const unsigned short *Image::getNoWiFi()
{
    return noWiFi;
}

const unsigned short *Image::getCurrentWeatherImage(String icon, ImageSize size)
{
    if (size == BIG)
    {
        return getCurrentWeatherBigImage(icon);
    }
    else
    {
        return getCurrentWeatherSmallImage(icon);
    }
}

const unsigned short *Image::getCurrentWeatherBigImage(String icon)
{
    if (icon == "01d")
    {
        return sunny_big;
    }
    else if (icon == "13d" || icon == "13n")
    {
        return snow_big;
    }
    else if (icon == "09d" || icon == "10d" || icon == "09n" || icon == "10n")
    {
        return rain_big;
    }
    else if (icon == "11d" || icon == "11n")
    {
        return lightning_big;
    }
    else if (icon == "04d" || icon == "50d" || icon == "04n" || icon == "50n")
    {
        return cloudy_big;
    }
    else if (icon == "03d")
    {
        return sun_most_cloudy_big;
    }
    else if (icon == "02d")
    {
        return sun_and_clouds_big;
    }
    else if (icon == "01n")
    {
        return moon_big;
    }
    else if (icon == "03n")
    {
        return moon_most_cloudy_big;
    }
    else if (icon == "02n")
    {
        return moon_cloudy_big;
    }
    else
    {
        return unknown_big;
    }
}

const unsigned short *Image::getCurrentWeatherSmallImage(String icon)
{
    if (icon == "01d")
    {
        return sunny_small;
    }
    else if (icon == "02d")
    {
        return sun_and_clouds_small;
    }
    else if (icon == "03d")
    {
        return sun_most_cloudy_small;
    }
    else if (icon == "04d" || icon == "50d")
    {
        return cloudy_small;
    }
    else if (icon == "09d" || icon == "10d")
    {
        return rain_small;
    }
    else if (icon == "11d")
    {
        return lightning_small;
    }
    else if (icon == "13d")
    {
        return snow_small;
    }
    else
    {
        return unknown_small;
    }
}
