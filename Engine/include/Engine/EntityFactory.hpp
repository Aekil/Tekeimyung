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

#define ARCHETYPES_LOCATION "resources/archetypes"

class IComponentFactory;

class EntityFactory
{
private:
    struct sEntityInfo
    {
        std::string tag;
        std::list<std::string> components;
    };

public:
    EntityFactory();
    ~EntityFactory();
    static void                                             loadDirectory(const std::string& archetypesDir);

    static Entity*                                          createOrGetEntity(const std::string& typeName);

    static Entity*                                          createEntity(const std::string& typeName, bool store = true);
    static Entity*                                          createEntity(const std::string& typeName, const glm::vec3& pos, bool store = true);

    static void                                             bindEntityManager(EntityManager* em);
    static EntityManager*                                   getBindedEntityManager();

    static void                                             createEntityType(const std::string& typeName);

    static const std::vector<const char*>&                  getTypesString();
    static bool                                             entityTypeExists(const std::string& type);

    static const sEntityInfo&                               getInfos(const std::string& typeName);
    static const void                                       removeComponent(const std::string& typeName, const std::string& component);
    static const void                                       addComponent(const std::string& typeName, const std::string& component);
    static const void                                       setTag(const std::string& typeName, const std::string& tag);
    static const std::string&                               getFile(const std::string& typeName);

    static void                                             updateEntitiesComponents(Entity* from, const std::string& entityName, IComponentFactory* compFactory, sComponent* component);

    static void                                             initAnimations(Entity* entity);
    static void                                             reverseAnimations(Entity* entity);

    static Entity*                                          cloneEntity(const std::string& typeName, bool store = true);

    static void                                             copyEntityManager(EntityManager* dst, EntityManager* src);

    static void                                             saveEntityTemplateToJson(const std::string& typeName);
    static void                                             saveEntityTemplate(const std::string& typeName, Entity* entity);

private:
    // Store entities components names (ComponentFactory has components)
    static std::unordered_map<std::string, sEntityInfo>     _entities;
    static std::vector<const char*>                         _typesString;
    static EntityManager*                                   _em;

    // Entity types file definition
    static std::unordered_map<std::string, std::string>     _entitiesFiles;
};
