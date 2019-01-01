#pragma once

#include "components.hh"
#include "planet.hh"
#include "types.hh"

#include <cassert>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
namespace Oso {

class World {
public:
  World();

  // factories

  entity_id addPlanet(const std::string &name, mass_t m);
  entity_id addOrbitingPlanet(const std::string &name, mass_t m, id_t parent,
                              const Orbit &orbit);
  entity_id addAsteroid(const std::string &name, mass_t m, id_t parent,
                        const velocity_v &vel, const location_t &loc);

  // getters

  Planet &getPlanet(entity_id id) {
    auto it = _planets.find(id);
    assert(it != _planets.end());
    return it->second;
  }
  location_t &getPosition(entity_id id) { return _positions[id]; }

  const Comp::GravitySource &getGravitySource(entity_id id) const {
    return _gravitySources.at(id);
  }

  // visitors
  template <typename F> void visitPlanets(F f) {
    for (auto &[id, planet] : _planets) {
      f(_positions[id], planet);
    }
  }

  template <typename F> void visitForcedOrbits(F f) {
    for (auto &[id, orbit] : _forcedOrbits) {
      f(_positions[id], orbit);
    }
  }

  template <typename F> void visitPhysSimulated(F f) {
    for (auto &[id, simulated] : _physSimulated) {
      f(_positions[id], simulated);
    }
  }

  template <typename F> void visitGravitySources(F f) {
    visitLocationAndContainer(f, _gravitySources);
  }

private:
  struct entity_t {
    std::string name;
  };

  template <typename F, typename Cont>
  void visitLocationAndContainer(F f, Cont &cont) {
    for (auto &[id, s] : cont) {
      f(_positions[id], s);
    }
  }

  entity_id createEntity(const std::string &name);

  std::vector<entity_t> _entities;
  std::vector<location_t> _positions;
  std::unordered_map<entity_id, Planet> _planets;
  std::unordered_map<entity_id, Comp::GravitySource> _gravitySources;
  std::unordered_map<entity_id, Comp::ForcedOrbit> _forcedOrbits;
  std::unordered_map<entity_id, Comp::PhysSimulated> _physSimulated;
};

} // namespace Oso
