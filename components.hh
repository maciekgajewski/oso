#pragma once

#include "orbit.hh"
#include "types.hh"

namespace Oso {

using entity_id = std::uint64_t;

struct GravitySource {
    mass_t mass;
};

struct ForcedOrbit {
    entity_id parent;
    Orbit orbit;
};
}
