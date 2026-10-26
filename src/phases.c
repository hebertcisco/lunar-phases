#include "phases.h"

#include <math.h>

#include "astronomy.h"

#define SYNODIC_MONTH_DAYS 29.530588861
#define ELONGATION_RATE_DEGREES_PER_DAY 12.19074939

static double wrap_degrees(double value)
{
    value = fmod(value, 360.0);
    if (value < 0.0) value += 360.0;
    return value > 180.0 ? value - 360.0 : value;
}

double phase_time_terrestrial(int lunation, MoonPhase phase)
{
    const double k = (double)lunation + (double)phase / 4.0;
    double julian_day = 2451550.09766 + SYNODIC_MONTH_DAYS*k;
    const double target = 90.0*(double)phase;

    for (int iteration = 0; iteration < 60; iteration++) {
        const double error = wrap_degrees(lunar_state_at_terrestrial_time(julian_day).elongation_degrees - target);
        julian_day -= error / ELONGATION_RATE_DEGREES_PER_DAY;
        if (fabs(error) < 1.0e-7) break;
    }
    return julian_day;
}

const char *phase_name(MoonPhase phase)
{
    static const char *names[] = { "New Moon", "First Quarter", "Full Moon", "Last Quarter" };
    return (phase >= NEW_MOON && phase <= LAST_QUARTER) ? names[phase] : "Unknown Phase";
}

const char *current_phase_name(double elongation)
{
    static const char *names[] = {
        "New", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
        "Full", "Waning Gibbous", "Last Quarter", "Waning Crescent"
    };
    int index = (int)floor((elongation + 22.5) / 45.0) % 8;
    if (index < 0) index += 8;
    return names[index];
}

