#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "astronomy.h"
#include "calendar.h"
#include "phases.h"

static void test_julian_day_reference(void)
{
    assert(fabs(calendar_to_julian_day(2000, 1, 1.5) - 2451545.0) < 1.0e-9);
    CalendarDateTime date_time;
    julian_day_to_calendar(2451545.0, &date_time);
    assert(date_time.year == 2000 && date_time.month == 1 && date_time.day == 1);
    assert(date_time.hour == 12 && date_time.minute == 0 && date_time.second == 0);
}

static void test_lunar_state_is_bounded(void)
{
    LunarState state = lunar_state_at_terrestrial_time(2451545.0);
    assert(state.lunar_distance_km > 350000.0 && state.lunar_distance_km < 410000.0);
    assert(state.illuminated_fraction >= 0.0 && state.illuminated_fraction <= 1.0);
    assert(state.elongation_degrees >= 0.0 && state.elongation_degrees < 360.0);
}

static void test_phase_solver(void)
{
    const double new_moon = phase_time_terrestrial(0, NEW_MOON);
    const double full_moon = phase_time_terrestrial(0, FULL_MOON);
    assert(full_moon > new_moon);
    assert(full_moon - new_moon > 13.0 && full_moon - new_moon < 16.0);
    assert(phase_name(FULL_MOON)[0] == 'F');
}

int main(void)
{
    test_julian_day_reference();
    test_lunar_state_is_bounded();
    test_phase_solver();
    puts("All lunar tests passed.");
    return 0;
}

