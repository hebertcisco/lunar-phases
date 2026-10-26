#ifndef LUNAR_CALENDAR_H
#define LUNAR_CALENDAR_H

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} CalendarDateTime;

double calendar_to_julian_day(int year, int month, double day);
void julian_day_to_calendar(double julian_day, CalendarDateTime *date_time);
double julian_day_to_decimal_year(double julian_day);
double delta_t_seconds(double decimal_year);
double universal_time_to_terrestrial_time(double julian_day_ut);
double terrestrial_time_to_universal_time(double julian_day_tt);

#endif

