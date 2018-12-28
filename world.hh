#pragma once

#include "planet.hh"
#include "types.hh"

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Oso {

class World {
public:
    using id_t = std::uint64_t;

    World();

    id_t addPlanet(const std::string& name);
    Planet& getPlanet(id_t id)
    {
        auto it = _planets.find(id);
        assert(it != _planets.end());
        return it->second;
    }
    location_t& getPosition(id_t id) { return _positions[id]; }

    template <typename F>
    void visitPlanets(F f)
    {
        for (id_t id = 0; id < _entities.size(); ++id) {
            f(_positions[id], _planets[id]);
        }
    }

private:
    struct entity_t {
        std::string name;
    };

    id_t createEntity(const std::string& name);

    std::vector<entity_t> _entities;
    std::vector<location_t> _positions;
    std::unordered_map<id_t, Planet> _planets;
};

} // namespace Oso
