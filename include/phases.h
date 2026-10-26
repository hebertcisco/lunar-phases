#ifndef LUNAR_PHASES_H
#define LUNAR_PHASES_H

typedef enum {
    NEW_MOON = 0,
    FIRST_QUARTER = 1,
    FULL_MOON = 2,
    LAST_QUARTER = 3
} MoonPhase;

double phase_time_terrestrial(int lunation, MoonPhase phase);
const char *phase_name(MoonPhase phase);
const char *current_phase_name(double elongation_degrees);

#endif

