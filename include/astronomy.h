#ifndef LUNAR_ASTRONOMY_H
#define LUNAR_ASTRONOMY_H

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
} LunarState;

LunarState lunar_state_at_terrestrial_time(double julian_day_tt);

#endif

