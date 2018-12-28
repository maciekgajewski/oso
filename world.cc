#include "world.hh"

namespace Oso {

World::World()
{
}

World::id_t World::addPlanet(const std::string& name)
{
    id_t id = createEntity(name);
    _planets[id] = Planet();
    return id;
}

World::id_t World::createEntity(const std::string& name)
{
    _entities.push_back(entity_t{ name });
    _positions.emplace_back();
    return id_t(_entities.size() - 1);
}

} // namespace Oso
