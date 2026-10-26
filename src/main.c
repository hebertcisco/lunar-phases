#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "astronomy.h"
#include "calendar.h"
#include "phases.h"

static double timezone_offset_hours(void)
{
    const char *value = getenv("TIMEZONE_OFFSET_HOURS");
    return value == NULL ? 0.0 : strtod(value, NULL);
}

static const char *timezone_label(double offset)
{
    static char label[32];
    if (offset == 0.0) return "UTC";
    (void)snprintf(label, sizeof label, "UTC%+.2g", offset);
    return label;
}

static void print_date(double julian_day_ut, double offset)
{
    CalendarDateTime date_time;
    julian_day_to_calendar(julian_day_ut + offset / 24.0, &date_time);
    (void)printf("%04d-%02d-%02d %02d:%02d:%02d", date_time.year, date_time.month,
        date_time.day, date_time.hour, date_time.minute, date_time.second);
}

static void print_state(double julian_day_ut, double offset)
{
    const double julian_day_tt = universal_time_to_terrestrial_time(julian_day_ut);
    const LunarState state = lunar_state_at_terrestrial_time(julian_day_tt);
    int lunation = (int)floor((julian_day_tt - 2451550.09766) / 29.530588861);
    double previous_new_moon = phase_time_terrestrial(lunation, NEW_MOON);
    if (previous_new_moon > julian_day_tt) {
        lunation--;
        previous_new_moon = phase_time_terrestrial(lunation, NEW_MOON);
    }

    (void)printf("Date/time (%s)       : ", timezone_label(offset)); print_date(julian_day_ut, offset); putchar('\n');
    (void)printf("Julian day (UT)      : %.6f\n", julian_day_ut);
    (void)printf("Delta T              : %.2f s\n", delta_t_seconds(julian_day_to_decimal_year(julian_day_ut)));
    (void)printf("Phase                : %s\n", current_phase_name(state.elongation_degrees));
    (void)printf("Illuminated fraction  : %.2f %%\n", state.illuminated_fraction * 100.0);
    (void)printf("Moon-Sun elongation  : %.4f degrees\n", state.elongation_degrees);
    (void)printf("Phase angle          : %.4f degrees\n", state.phase_angle_degrees);
    (void)printf("Moon age             : %.3f days\n", julian_day_tt - previous_new_moon);
    (void)printf("Moon ecliptic lon.   : %.4f degrees\n", state.lunar_longitude_degrees);
    (void)printf("Moon ecliptic lat.   : %.4f degrees\n", state.lunar_latitude_degrees);
    (void)printf("Distance             : %.0f km\n", state.lunar_distance_km);
    (void)printf("Angular diameter     : %.2f arcmin\n", state.lunar_angular_diameter_arcminutes);
    (void)printf("Sun ecliptic lon.    : %.4f degrees\n", state.solar_longitude_degrees);
    (void)printf("Previous new moon    : "); print_date(terrestrial_time_to_universal_time(previous_new_moon), offset); putchar('\n');
    (void)printf("Next new moon        : "); print_date(terrestrial_time_to_universal_time(phase_time_terrestrial(lunation + 1, NEW_MOON)), offset); putchar('\n');
}

static void print_year_phases(int year, double offset)
{
    const int first_lunation = (int)floor((year - 2000) * 12.3685) - 2;
    (void)printf("Lunar phases in %d (%s)\n", year, timezone_label(offset));
    (void)printf("----------------------------------------------\n");
    for (int lunation = first_lunation; lunation < first_lunation + 17; lunation++) {
        for (int phase = NEW_MOON; phase <= LAST_QUARTER; phase++) {
            const double julian_day_ut = terrestrial_time_to_universal_time(phase_time_terrestrial(lunation, (MoonPhase)phase));
            CalendarDateTime date_time;
            julian_day_to_calendar(julian_day_ut + offset / 24.0, &date_time);
            if (date_time.year != year) continue;
            (void)printf("%02d/%02d %02d:%02d:%02d  %-16s", date_time.day, date_time.month,
                date_time.hour, date_time.minute, date_time.second, phase_name((MoonPhase)phase));
            if (phase == NEW_MOON || phase == FULL_MOON) {
                const LunarState state = lunar_state_at_terrestrial_time(universal_time_to_terrestrial_time(julian_day_ut));
                (void)printf("  distance %.0f km", state.lunar_distance_km);
            }
            putchar('\n');
        }
    }
}

static void print_usage(const char *program)
{
    (void)fprintf(stderr, "Usage: %s\n       %s YEAR MONTH DAY [HOUR [MINUTE [SECOND]]]\n       %s phases YEAR\n", program, program, program);
}

int main(int argc, char **argv)
{
    const double offset = timezone_offset_hours();
    if (argc == 3 && strcmp(argv[1], "phases") == 0) {
        print_year_phases((int)strtol(argv[2], NULL, 10), offset);
        return 0;
    }
    if (argc >= 4) {
        const int year = (int)strtol(argv[1], NULL, 10);
        const int month = (int)strtol(argv[2], NULL, 10);
        const double day = strtod(argv[3], NULL);
        const double hour = argc > 4 ? strtod(argv[4], NULL) : 0.0;
        const double minute = argc > 5 ? strtod(argv[5], NULL) : 0.0;
        const double second = argc > 6 ? strtod(argv[6], NULL) : 0.0;
        const double local_julian_day = calendar_to_julian_day(year, month, day) + (hour + minute / 60.0 + second / 3600.0) / 24.0;
        print_state(local_julian_day - offset / 24.0, offset);
        return 0;
    }
    if (argc == 1) {
        print_state(2440587.5 + (double)time(NULL) / 86400.0, offset);
        return 0;
    }
    print_usage(argv[0]);
    return 1;
}

