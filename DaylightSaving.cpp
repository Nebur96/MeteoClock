#include "DaylightSaving.h"

DaylightSaving::DaylightSaving() {};

void DaylightSaving::calculateDaySavingTime(int hour, int day, int month, int year) {
  if (month < 3) {
    timeZone = 0;
  } else if (month == 3) {
    int lastSundayOfMonth = calculateLastDayOfMonth(year, 3);
    if (day == lastSundayOfMonth && hour >= 2 || day > lastSundayOfMonth) {
      timeZone = 1;
    } else {
      timeZone = 0;
    }
  } else if (month < 10) {
    timeZone = 1;
  } else if (month == 10) {
    int lastSundayOfMonth = calculateLastDayOfMonth(year, 3);
    if (day == lastSundayOfMonth && hour >= 2 || day > lastSundayOfMonth) {
      timeZone = 0;
    } else {
      timeZone = 1;
    }
  } else if (month < 12) {
    timeZone = 0;
  }
}

int DaylightSaving::calculateLastDayOfMonth(int year, int month) {
  int d = 31;
  int y = year;
  int day = 31 - ((d += month < 3 ? y-- : y - 2, 23 * month / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7);
  return day;
}
