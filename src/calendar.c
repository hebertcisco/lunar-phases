#include "calendar.h"

#include <math.h>

double calendar_to_julian_day(int year, int month, double day)
{
    const int gregorian = !(year < 1582 ||
        (year == 1582 && (month < 10 || (month == 10 && day < 15))));
    if (month <= 2) { year--; month += 12; }

    int correction = 0;
    if (gregorian) {
        const int century = year / 100;
        correction = 2 - century + century / 4;
    }
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1))
        + day + correction - 1524.5;
}

void julian_day_to_calendar(double julian_day, CalendarDateTime *date_time)
{
    const double rounded_day = floor(julian_day * 86400.0 + 0.5) / 86400.0;
    const double shifted_day = rounded_day + 0.5;
    const double z = floor(shifted_day);
    const double fraction = shifted_day - z;
    double a;

    if (z < 2299161.0) a = z;
    else {
        const double alpha = floor((z - 1867216.25) / 36524.25);
        a = z + 1.0 + alpha - floor(alpha / 4.0);
    }

    const double b = a + 1524.0;
    const double c = floor((b - 122.1) / 365.25);
    const double d = floor(365.25 * c);
    const double e = floor((b - d) / 30.6001);
    const int month = (e < 14.0) ? (int)e - 1 : (int)e - 13;
    const long seconds = (long)floor(fraction * 86400.0 + 0.5);

    date_time->month = month;
    date_time->year = (month > 2) ? (int)c - 4716 : (int)c - 4715;
    date_time->day = (int)(b - d - floor(30.6001 * e));
    date_time->hour = (int)(seconds / 3600);
    date_time->minute = (int)((seconds % 3600) / 60);
    date_time->second = (int)(seconds % 60);
}

double julian_day_to_decimal_year(double julian_day)
{
    return 2000.0 + (julian_day - 2451545.0) / 365.25;
}

double delta_t_seconds(double year)
{
    double t;
    if (year < 1860.0 || year >= 2150.0) {
        const double u = (year - 1820.0) / 100.0;
        return -20.0 + 32.0 * u * u;
    }
    if (year < 1900.0) {
        t = year - 1860.0;
        return 7.62 + 0.5737*t - 0.251754*t*t + 0.01680668*t*t*t
            - 0.0004473624*pow(t, 4) + pow(t, 5)/233174.0;
    }
    if (year < 1920.0) {
        t = year - 1900.0;
        return -2.79 + 1.494119*t - 0.0598939*t*t + 0.0061966*t*t*t
            - 0.000197*pow(t, 4);
    }
    if (year < 1941.0) {
        t = year - 1920.0;
        return 21.20 + 0.84493*t - 0.076100*t*t + 0.0020936*t*t*t;
    }
    if (year < 1961.0) {
        t = year - 1950.0;
        return 29.07 + 0.407*t - t*t/233.0 + t*t*t/2547.0;
    }
    if (year < 1986.0) {
        t = year - 1975.0;
        return 45.45 + 1.067*t - t*t/260.0 - t*t*t/718.0;
    }
    if (year < 2005.0) {
        t = year - 2000.0;
        return 63.86 + 0.3345*t - 0.060374*t*t + 0.0017275*pow(t, 3)
            + 0.000651814*pow(t, 4) + 0.00002373599*pow(t, 5);
    }
    if (year < 2050.0) {
        t = year - 2000.0;
        return 62.92 + 0.32217*t + 0.005589*t*t;
    }
    return -20.0 + 32.0*pow((year - 1820.0)/100.0, 2)
        - 0.5628*(2150.0 - year);
}

double universal_time_to_terrestrial_time(double julian_day_ut)
{
    return julian_day_ut + delta_t_seconds(julian_day_to_decimal_year(julian_day_ut)) / 86400.0;
}

double terrestrial_time_to_universal_time(double julian_day_tt)
{
    return julian_day_tt - delta_t_seconds(julian_day_to_decimal_year(julian_day_tt)) / 86400.0;
}

