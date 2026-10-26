#ifndef LUNAR_WASM_API_H
#define LUNAR_WASM_API_H

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

LunarWasmState *lunar_wasm_state(double julian_day_ut);
double lunar_wasm_phase_time(int lunation, int phase);
double lunar_wasm_calendar_to_julian(int year, int month, double day);

#endif
