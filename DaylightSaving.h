#ifndef DaylightSaving_h
#define DaylightSaving_h

class DaylightSaving
{
  public:
    DaylightSaving();
    void calculateDaySavingTime(int hour, int day, int month, int year);
    int timeZone = 0;
  private:
    int calculateLastDayOfMonth(int year, int month);
};

#endif
