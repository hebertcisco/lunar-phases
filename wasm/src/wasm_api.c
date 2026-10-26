#include <math.h>

#include "astronomy.h"
#include "calendar.h"
#include "phases.h"

/* A stable, flat ABI for JavaScript. Values are IEEE-754 doubles in this order. */
typedef struct {
    double lunar_longitude_degrees;
    double lunar_latitude_degrees;
    double lunar_distance_km;
    double lunar_angular_diameter_arcminutes;
    double solar_longitude_degrees;
    double solar_distance_au;
    double elongation_degrees;
    double phase_angle_degrees;
    double illuminated_fraction;
    double moon_age_days;
    double delta_t_seconds;
    double julian_day_ut;
    double previous_new_moon_ut;
    double next_new_moon_ut;
    int phase_index;
} LunarWasmState;

static LunarWasmState state_buffer;

static int current_phase_index(double elongation)
{
    int index = (int)floor((elongation + 22.5) / 45.0) % 8;
    return index < 0 ? index + 8 : index;
}

LunarWasmState *lunar_wasm_state(double julian_day_ut)
{
    const double julian_day_tt = universal_time_to_terrestrial_time(julian_day_ut);
    const LunarState state = lunar_state_at_terrestrial_time(julian_day_tt);
    int lunation = (int)floor((julian_day_tt - 2451550.09766) / 29.530588861);
    double previous_new_moon = phase_time_terrestrial(lunation, NEW_MOON);
    if (previous_new_moon > julian_day_tt) {
        lunation--;
        previous_new_moon = phase_time_terrestrial(lunation, NEW_MOON);
    }
    state_buffer.lunar_longitude_degrees = state.lunar_longitude_degrees;
    state_buffer.lunar_latitude_degrees = state.lunar_latitude_degrees;
    state_buffer.lunar_distance_km = state.lunar_distance_km;
    state_buffer.lunar_angular_diameter_arcminutes = state.lunar_angular_diameter_arcminutes;
    state_buffer.solar_longitude_degrees = state.solar_longitude_degrees;
    state_buffer.solar_distance_au = state.solar_distance_au;
    state_buffer.elongation_degrees = state.elongation_degrees;
    state_buffer.phase_angle_degrees = state.phase_angle_degrees;
    state_buffer.illuminated_fraction = state.illuminated_fraction;
    state_buffer.moon_age_days = julian_day_tt - previous_new_moon;
    state_buffer.delta_t_seconds = delta_t_seconds(julian_day_to_decimal_year(julian_day_ut));
    state_buffer.julian_day_ut = julian_day_ut;
    state_buffer.previous_new_moon_ut = terrestrial_time_to_universal_time(previous_new_moon);
    state_buffer.next_new_moon_ut = terrestrial_time_to_universal_time(phase_time_terrestrial(lunation + 1, NEW_MOON));
    state_buffer.phase_index = current_phase_index(state.elongation_degrees);
    return &state_buffer;
}

double lunar_wasm_phase_time(int lunation, int phase)
{
    return terrestrial_time_to_universal_time(phase_time_terrestrial(lunation, (MoonPhase)phase));
}

double lunar_wasm_calendar_to_julian(int year, int month, double day)
{
    return calendar_to_julian_day(year, month, day);
}
