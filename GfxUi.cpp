#include "GfxUi.h"

GfxUi::GfxUi(Adafruit_ILI9341 *tft) {
  _tft = tft;
}

void GfxUi::drawString(int x, int y, char *text) {
  int16_t x1, y1;
  uint16_t w, h;
  _tft->setTextWrap(false);
  _tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  switch (_alignment) {
    case LEFT:
      x1 = x;
      _tft->fillRect(x1, y, 240, h, ILI9341_BLACK);
      yield();
      break;
    case CENTER:
      x1 = x - w / 2;
      _tft->fillRect(0, y, 240, h, ILI9341_BLACK);
      yield();
      break;
    case RIGHT:
      x1 = x - w;
      _tft->fillRect(0, y, x1 + w, h, ILI9341_BLACK);
      yield();
      break;
  }

  _tft->setCursor(x1, y);
  _tft->print(text);
}

void GfxUi::drawString(int x, int y, String text) {
  char buf[text.length() + 2];
  text.toCharArray(buf, text.length() + 1);
  drawString(x, y, buf);
}

void GfxUi::drawStringInInterval(int x_start, int x_final, int y, char *text) {
  int16_t x1, y1;
  uint16_t w, h;

  _tft->setTextWrap(false);
  _tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int16_t pos = ((x_final - x_start - w) / 2) + x_start;
  _tft->setCursor(pos, y);
  _tft->print(text);
}

void GfxUi::drawStringInInterval(int x_start, int x_final, int y, String text) {
  char buf[text.length() + 2];
  text.toCharArray(buf, text.length() + 1);
  drawStringInInterval(x_start, x_final, y, buf);
}

void GfxUi::setTextColor(uint16_t c) {
  setTextColor(c, c);
}
void GfxUi::setTextColor(uint16_t c, uint16_t bg) {
  _textColor = c;
  _backgroundColor = bg;
  _tft->setTextColor(_textColor, _backgroundColor);
}

void GfxUi::setTextAlignment(TextAlignment alignment) {
  _alignment = alignment;
}

void GfxUi::drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor) {
  if (percentage == 0) {
    _tft->fillRoundRect(x0, y0, w, h, 3, _backgroundColor);
  }
  uint8_t margin = 2;
  uint16_t barHeight = h - 2 * margin;
  uint16_t barWidth = w - 2 * margin;
  _tft->drawRoundRect(x0, y0, w, h, 3, frameColor);
  _tft->fillRect(x0 + margin, y0 + margin, barWidth * percentage / 100.0, barHeight, barColor);
}

