#pragma once

#include <boost/operators.hpp>

#include <chrono>
#include <ostream>

namespace Oso {

using tick_t = std::chrono::duration<std::int64_t, std::milli>;
using length_t = double; // consider strong typedef
using distnce_t = length_t;
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
template <typename T>
v2<T> operator+(const v2<T>& a, const v2<T>& b) { return { a.x + b.x, a.y + b.y }; };

template <typename T>
std::ostream& operator<<(std::ostream& o, const v2<T>& v) { return o << v.x << ", " << v.y; }

using position_t = v2<length_t>;

struct location_t {
    position_t pos;
    angle_t a = 0.0;
};
}
