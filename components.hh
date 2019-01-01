#pragma once

#include "orbit.hh"
#include "types.hh"

namespace Oso {

using entity_id = std::uint64_t;

namespace Comp {

struct GravitySource {
  mass_t mass;
};

struct ForcedOrbit {
  entity_id parent;
  Orbit orbit;
};

struct PhysSimulated {

  v2<accel_t> accel;
  v2<velocity_t> velocity;
};

} // namespace Comp
} // namespace Oso
