/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/ComponentFactory.hpp>

// Generate a list
// string: "PLAYER", "TILE1", "TILE2"
// enum: PLAYER, TILE1, TILE2
#define ENTITIES_TYPES(PROCESS)\
    PROCESS(EMPTY_OBJECT),\
    PROCESS(PLAYER),\
    PROCESS(ENEMY),\
    PROCESS(BLOCK_GREEN),\
    PROCESS(BLOCK_BROWN),\
    PROCESS(SPAWNER),\
    PROCESS(TOWER_FIRE),\
    PROCESS(EMITTER_FIRE),\
    PROCESS(EMITTER_WATER),\
    PROCESS(FIRE_BALL),\
    PROCESS(ICON_SELECTED), \
    PROCESS(BUTTON_RETURN), \
    PROCESS(BUTTON_RESUME),\
    PROCESS(BUTTON_QUIT),\
    PROCESS(BUTTON_TOGGLE_FULLSCREEN),\
    PROCESS(BUTTON_TOGGLE_WINDOWED),\
    PROCESS(BUTTON_HOW_TO_PLAY),\
    PROCESS(BUTTON_OPTIONS),\
    PROCESS(HOW_TO_PLAY_MENU),\
    PROCESS(BACKGROUND),\
    PROCESS(BUTTON_CONFIRM_YES),\
    PROCESS(BUTTON_CONFIRM_NO),\
    PROCESS(QUIT_CONFIRM_POPUP),\
    PROCESS(QUIT_CONFIRM_TEXT),\
    PROCESS(GAME_MANAGER),\
    PROCESS(CASTLE),\
    PROCESS(TRAP_NEEDLE),\
    PROCESS(TRAP_CUTTER),\
    PROCESS(TRAP_FIRE),\

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

#define ARCHETYPES_LOCATION "resources/archetypes"

class IComponentFactory;

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
    static Entity*                                          createEntity(eArchetype type, const glm::vec3& pos);
    static Entity*                                          createEntity(const std::string& typeName);
    static Entity*                                          createEntity(const std::string& typeName, const glm::vec3& pos);
    static void                                             bindEntityManager(EntityManager* em);
    static EntityManager*                                   getBindedEntityManager();

    static Entity*                                          createEntityType(const std::string& typeName);

    static const std::vector<const char*>&                  getTypesString();
    static bool                                             entityTypeExists(const std::string& type);

    static const std::list<std::string>&                    getComponents(const std::string& typeName);
    static const void                                       removeComponent(const std::string& typeName, const std::string& component);
    static const void                                       addComponent(const std::string& typeName, const std::string& component);
    static const std::string&                               getFile(const std::string& typeName);

    static void                                             updateEntitiesComponents(Entity* from, const std::string& entityName, IComponentFactory* compFactory, sComponent* component);

    static void                                             initAnimations(Entity* entity);
    static void                                             reverseAnimations(Entity* entity);

    static Entity*                                          cloneEntity(Entity* entity);
    static Entity*                                          cloneEntity(const std::string& typeName);

private:
    // Store entities components names (ComponentFactory has components)
    static std::unordered_map<std::string, std::list<std::string>>         _entities;
    static std::vector<const char*>                         _typesString;
    static EntityManager*                                   _em;

    // Entity types file definition
    static std::unordered_map<std::string, std::string>     _entitiesFiles;
};
