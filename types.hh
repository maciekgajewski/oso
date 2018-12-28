#pragma once

#include <boost/operators.hpp>

#include <chrono>

namespace Oso {

using tick_t = std::chrono::duration<std::int64_t, std::milli>;
using length_t = double; // consider strong typedef
using angle_t = double;
using mass_t = double;

using velocity_t = double;
using angular_velocity_t = double;

template <typename T>
struct v2 {
    T x = 0.0;
    T y = 0.0;
};

template <typename T>
v2<T> operator*(double r, const v2<T>& v) { return { r * v.x, r * v.y }; }

using position_t = v2<length_t>;

struct location_t : position_t {
    angle_t a = 0.0;
};
}
