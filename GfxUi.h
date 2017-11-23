#include <Adafruit_ILI9341.h>
#include <FS.h>

#ifndef _GFX_UI_H
#define _GFX_UI_H


#define BUFFPIXEL 20

enum TextAlignment {
  LEFT, CENTER, RIGHT
};

class GfxUi {
  public:
    GfxUi(Adafruit_ILI9341 * tft);
    void drawString(int x, int y, char *text);
    void drawString(int x, int y, String text);
    void drawStringInInterval(int x_start, int x_final, int y, char *text);
    void drawStringInInterval(int x_start, int x_final, int y, String text);
    void setTextAlignment(TextAlignment alignment);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void drawProgressBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor);

  private:
    Adafruit_ILI9341 * _tft;
    TextAlignment _alignment = LEFT;
    uint16_t _textColor;
    uint16_t _backgroundColor;
};

#endif

