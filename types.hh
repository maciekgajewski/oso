#pragma once

#include <chrono>

namespace Oso {

using tick_t = std::chrono::duration<std::int64_t, std::milli>;
using length_t = double; // consider strong typedef
using angle_t = double;

}
