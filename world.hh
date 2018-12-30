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

    entity_id addPlanet(const std::string& name, mass_t m);
    entity_id addOrbitingPlanet(const std::string& name, mass_t m, id_t parent, const Orbit& orbit);
    Planet& getPlanet(entity_id id)
    {
        auto it = _planets.find(id);
        assert(it != _planets.end());
        return it->second;
    }
    location_t& getPosition(entity_id id) { return _positions[id]; }

    const GravitySource& getGravitySource(entity_id id) const { return _gravitySources.at(id); }

    template <typename F>
    void visitPlanets(F f)
    {
        for (auto& [id, planet] : _planets) {
            f(_positions[id], planet);
        }
    }

    template <typename F>
    void visitForcedOrbits(F f)
    {
        for (auto& [id, orbit] : _forcedOrbits) {
            f(_positions[id], orbit);
        }
    }

private:
    struct entity_t {
        std::string name;
    };

    entity_id createEntity(const std::string& name);

    std::vector<entity_t> _entities;
    std::vector<location_t> _positions;
    std::unordered_map<entity_id, Planet> _planets;
    std::unordered_map<entity_id, GravitySource> _gravitySources;
    std::unordered_map<entity_id, ForcedOrbit> _forcedOrbits;
};

} // namespace Oso
