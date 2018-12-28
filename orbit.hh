#pragma once

#include "types.hh"

namespace Oso {

class Orbit {
public:
    // orbital elements, invariant when no force applied
    length_t a; // semi-major axis
    double e; // eccentricity (0=circle, 0-1=ellipse, 1=parabola)
    angle_t longitudeOfPeriapsis;

    // position on the orbit:
    // TODO mean anomaly at epoch, or time of periapsis passage
    angle_t M0; // mean anomaly at epoch

    position_t findPosition(tick_t time, mass_t m, mass_t M) const;
};

} // namespace Oso
