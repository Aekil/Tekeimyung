#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Entity.hpp"

// Generate a list
// string: "PLAYER", "TILE1", "TILE2"
// enum: PLAYER, TILE1, TILE2
#define ENTITIES_TYPES(PROCESS)\
    PROCESS(PLAYER),\
    PROCESS(TILE1),\
    PROCESS(TILE2),\
    PROCESS(TILE3),\

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
    static Entity*                                          createEntity(eArchetype& type);

private:
    static bool                                             entityTypeExists(const std::string& type);

private:
    static std::unordered_map<std::string, Entity*>         _entities;
    static std::vector<std::string>                         _typesString;
};
