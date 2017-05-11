#include <Engine/EntityFactory.hpp>

#include <Game/Map/Map.hpp>

std::unordered_map<int, std::vector<Entity*>>&  Map::getMapParts()
{
    return (_mapParts);
}

DoubleArray<char>&  Map::getSpawnersPaths()
{
    return (_spawnersPaths);
}

DoubleArray<Entity*>&  Map::getEntities()
{
    return (_entities);
}

const glm::ivec2&   Map::getCastlePos() const
{
    return (_castlePos);
}

void    Map::init()
{
    _entities.allocate(_width, _height);
    _spawnersPaths.allocate(_width, _height);
    for (int x = 0; x < _width; ++x)
    {
        for (int z = 0; z < _height; ++z)
        {
            if ((*this)[x][z] == -1)
            {
                // TODO: display other entity for castle pos ?
                _entities[x][z] = EntityFactory::createEntity("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                _castlePos = {x, z};
                continue;
            }
            if ((*this)[x][z] / LAYER_NUMBER == 0)
            {
                if ((*this)[x][z] % LAYER_NUMBER == 1)
                    _entities[x][z] = EntityFactory::createEntity("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                else if ((*this)[x][z] % LAYER_NUMBER == 2)
                    _entities[x][z] = EntityFactory::createEntity("SPAWNER", glm::vec3(x * 25, 0, z * 25));
                else if ((*this)[x][z] % LAYER_NUMBER == 3)
                    _entities[x][z] = EntityFactory::createEntity("BUILDABLE_TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
            }
            else
            {
                Entity* entity = nullptr;

                if ((*this)[x][z] % LAYER_NUMBER == 1)
                {
                    entity = EntityFactory::createEntity("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                }
                else if ((*this)[x][z] % LAYER_NUMBER == 2)
                {
                    entity = EntityFactory::createEntity("SPAWNER", glm::vec3(x * 25, 0, z * 25));

                    entity->getComponent<sScriptComponent>()->enabled = false;
                }

                if (!entity)
                {
                    continue;
                }

                entity->getComponent<sRenderComponent>()->enabled = false;

                _entities[x][z] = entity;
                _mapParts[(*this)[x][z] / LAYER_NUMBER].push_back(entity);
            }
        }
    }
}
