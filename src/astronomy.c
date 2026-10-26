#include "astronomy.h"

#include <math.h>
#include <stdlib.h>

#include "calendar.h"

#define PI 3.14159265358979323846
#define DEGREES_TO_RADIANS (PI / 180.0)
#define ASTRONOMICAL_UNIT_KM 149597870.7
#define MOON_RADIUS_KM 1737.4

typedef struct { int d, m, moon_m, f, longitude, distance; } LongitudeTerm;
typedef struct { int d, m, moon_m, f, latitude; } LatitudeTerm;

static const LongitudeTerm longitude_terms[] = {
 {0,0,1,0,6288774,-20905355},{2,0,-1,0,1274027,-3699111},{2,0,0,0,658314,-2955968},{0,0,2,0,213618,-569925},
 {0,1,0,0,-185116,48888},{0,0,0,2,-114332,-3149},{2,0,-2,0,58793,246158},{2,-1,-1,0,57066,-152138},
 {2,0,1,0,53322,-170733},{2,-1,0,0,45758,-204586},{0,1,-1,0,-40923,-129620},{1,0,0,0,-34720,108743},
 {0,1,1,0,-30383,104755},{2,0,0,-2,15327,10321},{0,0,1,2,-12528,0},{0,0,1,-2,10980,79661},
 {4,0,-1,0,10675,-34782},{0,0,3,0,10034,-23210},{4,0,-2,0,8548,-21636},{2,1,-1,0,-7888,24208},
 {2,1,0,0,-6766,30824},{1,0,-1,0,-5163,-8379},{1,1,0,0,4987,-16675},{2,-1,1,0,4036,-12831},
 {2,0,2,0,3994,-10445},{4,0,0,0,3861,-11650},{2,0,-3,0,3665,14403},{0,1,-2,0,-2689,-7003},
 {2,0,-1,2,-2602,0},{2,-1,-2,0,2390,10056},{1,0,1,0,-2348,6322},{2,-2,0,0,2236,-9884},
 {0,1,2,0,-2120,5751},{0,2,0,0,-2069,0},{2,-2,-1,0,2048,-4950},{2,0,1,-2,-1773,4130},
 {2,0,0,2,-1595,0},{4,-1,-1,0,1215,-3958},{0,0,2,2,-1110,0},{3,0,-1,0,-892,3258},
 {2,1,1,0,-810,2616},{4,-1,-2,0,759,-1897},{0,2,-1,0,-713,-2117},{2,2,-1,0,-700,2354},
 {2,1,-2,0,691,0},{2,-1,0,-2,596,0},{4,0,1,0,549,-1423},{0,0,4,0,537,-1117},
 {4,-1,0,0,520,-1571},{1,0,-2,0,-487,-1739},{2,1,0,-2,-399,0},{0,0,2,-2,-381,-4421},
 {1,1,1,0,351,0},{3,0,-2,0,-340,0},{4,0,-3,0,330,0},{2,-1,2,0,327,0},
 {0,2,1,0,-323,1165},{1,1,-1,0,299,0},{2,0,3,0,294,0},{2,0,-1,-2,0,8752}
};

static const LatitudeTerm latitude_terms[] = {
 {0,0,0,1,5128122},{0,0,1,1,280602},{0,0,1,-1,277693},{2,0,0,-1,173237},{2,0,-1,1,55413},{2,0,-1,-1,46271},
 {2,0,0,1,32573},{0,0,2,1,17198},{2,0,1,-1,9266},{0,0,2,-1,8822},{2,-1,0,-1,8216},{2,0,-2,-1,4324},
 {2,0,1,1,4200},{2,1,0,-1,-3359},{2,-1,-1,1,2463},{2,-1,0,1,2211},{2,-1,-1,-1,2065},{0,1,-1,-1,-1870},
 {4,0,-1,-1,1828},{0,1,0,1,-1794},{0,0,0,3,-1749},{0,1,-1,1,-1565},{1,0,0,1,-1491},{0,1,1,1,-1475},
 {0,1,1,-1,-1410},{0,1,0,-1,-1344},{1,0,0,-1,-1335},{0,0,3,1,1107},{4,0,0,-1,1021},{4,0,-1,1,833},
 {0,0,1,-3,777},{4,0,-2,1,671},{2,0,0,-3,607},{2,0,2,-1,596},{2,-1,1,-1,491},{2,0,-2,1,-451},
 {0,0,3,-1,439},{2,0,2,1,422},{2,0,-3,-1,421},{2,1,-1,1,-366},{2,1,0,1,-351},{4,0,0,1,331},
 {2,-1,1,1,315},{2,-2,0,-1,302},{0,0,1,3,-283},{2,1,1,-1,-229},{1,1,0,-1,223},{1,1,0,1,223},
 {0,1,-2,-1,-220},{2,1,-1,-1,-220},{1,0,1,1,-185},{2,-1,-2,-1,181},{0,1,2,1,-177},{4,0,-2,-1,176},
 {4,-1,-1,-1,166},{1,0,1,-1,-164},{4,0,1,-1,132},{1,0,-1,-1,-119},{4,-1,0,-1,115},{2,-2,0,1,107}
};

static double normalize_degrees(double value)
{
    value = fmod(value, 360.0);
    return value < 0.0 ? value + 360.0 : value;
}

static double lunar_longitude(double t, double *latitude, double *distance)
{
    const double mean_longitude = normalize_degrees(218.3164477 + t*(481267.88123421 + t*(-0.0015786 + t*(1.0/538841.0 + t*(-1.0/65194000.0)))));
    const double d = normalize_degrees(297.8501921 + t*(445267.1114034 + t*(-0.0018819 + t*(1.0/545868.0 + t*(-1.0/113065000.0)))));
    const double sun_anomaly = normalize_degrees(357.5291092 + t*(35999.0502909 + t*(-0.0001536 + t/24490000.0)));
    const double moon_anomaly = normalize_degrees(134.9633964 + t*(477198.8675055 + t*(0.0087414 + t*(1.0/69699.0 + t*(-1.0/14712000.0)))));
    const double f = normalize_degrees(93.2720950 + t*(483202.0175233 + t*(-0.0036539 + t*(-1.0/3526000.0 + t/863310000.0))));
    const double a1 = normalize_degrees(119.75 + 131.849*t), a2 = normalize_degrees(53.09 + 479264.290*t), a3 = normalize_degrees(313.45 + 481266.484*t);
    const double eccentricity = 1.0 - t*(0.002516 + 0.0000074*t);
    const double dr = d*DEGREES_TO_RADIANS, mr = sun_anomaly*DEGREES_TO_RADIANS, mpr = moon_anomaly*DEGREES_TO_RADIANS, fr = f*DEGREES_TO_RADIANS;
    double longitude_sum = 0.0, distance_sum = 0.0, latitude_sum = 0.0;
    for (size_t i = 0; i < sizeof longitude_terms / sizeof longitude_terms[0]; i++) {
        const LongitudeTerm term = longitude_terms[i];
        const double argument = term.d*dr + term.m*mr + term.moon_m*mpr + term.f*fr;
        const double factor = term.m == 0 ? 1.0 : (abs(term.m) == 1 ? eccentricity : eccentricity*eccentricity);
        longitude_sum += factor * term.longitude * sin(argument);
        distance_sum += factor * term.distance * cos(argument);
    }
    for (size_t i = 0; i < sizeof latitude_terms / sizeof latitude_terms[0]; i++) {
        const LatitudeTerm term = latitude_terms[i];
        const double argument = term.d*dr + term.m*mr + term.moon_m*mpr + term.f*fr;
        const double factor = term.m == 0 ? 1.0 : (abs(term.m) == 1 ? eccentricity : eccentricity*eccentricity);
        latitude_sum += factor * term.latitude * sin(argument);
    }
    longitude_sum += 3958.0*sin(a1*DEGREES_TO_RADIANS) + 1962.0*sin((mean_longitude-f)*DEGREES_TO_RADIANS) + 318.0*sin(a2*DEGREES_TO_RADIANS);
    latitude_sum += -2235.0*sin(mean_longitude*DEGREES_TO_RADIANS) + 382.0*sin(a3*DEGREES_TO_RADIANS) + 175.0*sin((a1-f)*DEGREES_TO_RADIANS) + 175.0*sin((a1+f)*DEGREES_TO_RADIANS) + 127.0*sin((mean_longitude-moon_anomaly)*DEGREES_TO_RADIANS) - 115.0*sin((mean_longitude+moon_anomaly)*DEGREES_TO_RADIANS);
    *latitude = latitude_sum / 1.0e6;
    *distance = 385000.56 + distance_sum / 1000.0;
    return normalize_degrees(mean_longitude + longitude_sum / 1.0e6);
}

static double nutation_longitude(double t)
{
    const double omega = (125.04452 - 1934.136261*t)*DEGREES_TO_RADIANS;
    const double sun_longitude = (280.4665 + 36000.7698*t)*DEGREES_TO_RADIANS;
    const double moon_longitude = (218.3165 + 481267.8813*t)*DEGREES_TO_RADIANS;
    return (-17.20*sin(omega) - 1.32*sin(2.0*sun_longitude) - 0.23*sin(2.0*moon_longitude) + 0.21*sin(2.0*omega)) / 3600.0;
}

static double solar_longitude(double t, double nutation, double *distance_au)
{
    const double mean_longitude = 280.46646 + t*(36000.76983 + t*0.0003032);
    const double mean_anomaly = (357.52911 + t*(35999.05029 - 0.0001537*t))*DEGREES_TO_RADIANS;
    const double eccentricity = 0.016708634 - t*(0.000042037 + 0.0000001267*t);
    const double equation = (1.914602 - t*(0.004817 + 0.000014*t))*sin(mean_anomaly) + (0.019993 - 0.000101*t)*sin(2.0*mean_anomaly) + 0.000289*sin(3.0*mean_anomaly);
    const double true_anomaly = mean_anomaly + equation*DEGREES_TO_RADIANS;
    *distance_au = 1.000001018*(1.0 - eccentricity*eccentricity)/(1.0 + eccentricity*cos(true_anomaly));
    return normalize_degrees(mean_longitude + equation + nutation - 20.4898/3600.0 / *distance_au);
}

LunarState lunar_state_at_terrestrial_time(double julian_day_tt)
{
    LunarState state;
    const double t = (julian_day_tt - 2451545.0) / 36525.0;
    const double nutation = nutation_longitude(t);
    state.lunar_longitude_degrees = lunar_longitude(t, &state.lunar_latitude_degrees, &state.lunar_distance_km) + nutation;
    state.lunar_longitude_degrees = normalize_degrees(state.lunar_longitude_degrees);
    state.lunar_angular_diameter_arcminutes = 2.0*asin(MOON_RADIUS_KM/state.lunar_distance_km)/DEGREES_TO_RADIANS*60.0;
    state.solar_longitude_degrees = solar_longitude(t, nutation, &state.solar_distance_au);
    state.elongation_degrees = normalize_degrees(state.lunar_longitude_degrees - state.solar_longitude_degrees);
    const double cosine_elongation = cos(state.lunar_latitude_degrees*DEGREES_TO_RADIANS)*cos(state.elongation_degrees*DEGREES_TO_RADIANS);
    const double elongation = acos(cosine_elongation);
    const double sun_distance_km = state.solar_distance_au*ASTRONOMICAL_UNIT_KM;
    state.phase_angle_degrees = atan2(sun_distance_km*sin(elongation), state.lunar_distance_km - sun_distance_km*cos(elongation))/DEGREES_TO_RADIANS;
    state.illuminated_fraction = (1.0 + cos(state.phase_angle_degrees*DEGREES_TO_RADIANS))/2.0;
    return state;
}

