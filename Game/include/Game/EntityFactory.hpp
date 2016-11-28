#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Game/ComponentFactory.hpp>

// Generate a list
// string: "PLAYER", "TILE1", "TILE2"
// enum: PLAYER, TILE1, TILE2
#define ENTITIES_TYPES(PROCESS)\
    PROCESS(PLAYER),\
    PROCESS(ENEMY),\
    PROCESS(BLOCK_GREEN),\
    PROCESS(BLOCK_BROWN),\
    PROCESS(WAVE_SPAWNER),\
    PROCESS(TOWER_FIRE),\
    PROCESS(EMITTER_FIRE),\
    PROCESS(EMITTER_WATER),\
    PROCESS(FIRE_BALL),\

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

#define ARCHETYPES_LOCATION "resources/archetypes"



enum class eArchetype
{
    ENTITIES_TYPES(GENERATE_ENUM)
};

class EntityFactory
{
public:
    EntityFactory();
    ~EntityFactory();
    static void                                             loadDirectory(const std::string& archetypesDir);
    static Entity*                                          createEntity(eArchetype type);
    static Entity*                                          createEntity(const std::string& typeName);
    static void                                             bindEntityManager(EntityManager* em);

    static void                                             createEntityType(const std::string& typeName);

    static const std::vector<const char*>&                  getTypesString();
    static bool                                             entityTypeExists(const std::string& type);

    static const std::list<std::string>&                    getComponents(const std::string& typeName);
    static const void                                       removeComponent(const std::string& typeName, const std::string& component);
    static const void                                       addComponent(const std::string& typeName, const std::string& component);
    static const std::string&                               getFile(const std::string& typeName);

    static void                                             updateEntityComponent(const std::string& entityName, IComponentFactory* compFactory, sComponent* component);

private:
    static Entity*                                          cloneEntity(const std::string& typeName);

private:
    // Store entities components names (ComponentFactory has components)
    static std::unordered_map<std::string, std::list<std::string>>         _entities;
    static std::vector<const char*>                         _typesString;
    static EntityManager*                                   _em;

    // Entity types file definition
    static std::unordered_map<std::string, std::string>     _entitiesFiles;
};
