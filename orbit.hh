#pragma once

#include "types.hh"

namespace Oso {

struct Orbit {
    // orbital elements, invariant when no force applied
    length_t a; // semi-major axis
    double e; // eccentricity (0=circle, 0-1=ellipse, 1=parabola)
    angle_t w; // longitude of peroapsis, e the direction of eccenticity vector

    // position on the orbit:
    // TODO mean anomaly at epoch, or time of periapsis passage
    angle_t M0; // mean anomaly at epoch

    position_t findPosition(tick_t time, mass_t m, mass_t M) const;

    static Orbit circular(distnce_t radius, angle_t M0);
};

} // namespace Oso
