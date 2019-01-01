#include "world.hh"

namespace Oso {

World::World() {}

entity_id World::addPlanet(const std::string &name, mass_t m) {
  entity_id id = createEntity(name);
  assert(_planets.find(id) == _planets.end());
  assert(_gravitySources.find(id) == _gravitySources.end());
  _planets[id] = Planet();
  _gravitySources[id] = Comp::GravitySource{m};
  return id;
}

entity_id World::addOrbitingPlanet(const std::string &name, mass_t m,
                                   id_t parent, const Orbit &orbit) {
  entity_id id = addPlanet(name, m);

  assert(_forcedOrbits.find(id) == _forcedOrbits.end());
  _forcedOrbits[id] = Comp::ForcedOrbit{parent, orbit};

  return id;
}

entity_id World::createEntity(const std::string &name) {
  _entities.push_back(entity_t{name});
  _positions.emplace_back();
  return entity_id(_entities.size() - 1);
}

entity_id World::addAsteroid(const std::string &name, mass_t m, id_t parent,
                             const velocity_v &vel, const location_t &loc) {
  entity_id id = createEntity(name);

  assert(_planets.find(id) == _planets.end());
  assert(_physSimulated.find(id) == _physSimulated.end());

  _planets[id] = Planet();
  _physSimulated[id] = Comp::PhysSimulated{accel_v{0, 0}, vel};
  _positions[id] = loc;
  _positions[id].pos += _positions[parent].pos;

  return id;
}

} // namespace Oso
