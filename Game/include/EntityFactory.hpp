#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Entity.hpp"
#include "EntityManager.hpp"

// Generate a list
// string: "PLAYER", "TILE1", "TILE2"
// enum: PLAYER, TILE1, TILE2
#define ENTITIES_TYPES(PROCESS)\
    PROCESS(PLAYER),\
    PROCESS(ENEMY),\
    PROCESS(BLOCK_GREEN),\
    PROCESS(BLOCK_BROWN),\
    PROCESS(TOWER_FIRE),\

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING




enum class eArchetype
{
    ENTITIES_TYPES(GENERATE_ENUM)
};

class EntityFactory
{
public:
    EntityFactory();
    ~EntityFactory();
    static void                                             init(const std::string& archetypesDir);
    static Entity*                                          createEntity(eArchetype type);
    static void                                             bindEntityManager(EntityManager* em);

private:
    static bool                                             entityTypeExists(const std::string& type);
    static Entity*                                          cloneEntity(const std::string& typeName);

private:
    // Store entities components names (ComponentFactory has components)
    static std::unordered_map<std::string, std::list<std::string>>         _entities;
    static std::vector<std::string>                         _typesString;
    static EntityManager*                                   _em;
};
