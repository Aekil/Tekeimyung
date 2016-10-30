#include <dirent.h>

#include "Utils/Exception.hpp"
#include "Utils/RessourceManager.hpp"
#include "Utils/Debug.hpp"
#include "Utils/JsonReader.hpp"
#include "Utils/Logger.hpp"
#include "ComponentFactory.hpp"

#include "EntityFactory.hpp"

std::unordered_map<std::string, std::list<std::string> >  EntityFactory::_entities;
std::unordered_map<std::string, std::string>  EntityFactory::_entitiesFiles;
std::vector<const char*>  EntityFactory::_typesString = { ENTITIES_TYPES(GENERATE_STRING) };
EntityManager*  EntityFactory::_em = nullptr;

EntityFactory::EntityFactory() {}

EntityFactory::~EntityFactory() {}

void EntityFactory::loadDirectory(const std::string& archetypesDir)
{
    DIR* dir;
    struct dirent* ent;
    RessourceManager* ressourceManager = RessourceManager::getInstance();

    dir = opendir(archetypesDir.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory \"%s\"", archetypesDir.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            // Get entity configuration file
            std::string path = std::string(archetypesDir).append("/").append(ent->d_name);
            std::string entityConf = ressourceManager->getFile(path);

            // Parse the configuration file with jsoncpp
            JsonReader jsonReader;
            JsonValue parsed;
            std::string typeName;
            if (!jsonReader.parse(entityConf, parsed))
                EXCEPT(IOException, "Cannot parse archetype \"%s\"", path.c_str());

            typeName = parsed.getString("name", "");
            LOG_INFO("Load entity %s", typeName.c_str());

            if (!EntityFactory::entityTypeExists(typeName)) // The macro ENTITIES_TYPES did not create the type
                EXCEPT(InvalidParametersException, "Failed to read entity archetype: Entity type \"%s\" does not exist", typeName.c_str());

            // Create entity components
            auto &&components = parsed.get("components", {}).get();
            for (Json::ValueIterator it = components.begin(); it != components.end(); it++)
            {
                std::string componentName = it.key().asString();

                // The macro COMPONENTS_TYPES did not create the type
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                    EXCEPT(InvalidParametersException, "Failed to read entity archetype: Component type \"%s\" does not exist", componentName.c_str());

                LOG_INFO("Add %s component %s", typeName.c_str(), componentName.c_str());
                IComponentFactory::initComponent(typeName, componentName, JsonValue(*it));
                _entitiesFiles[typeName] = path;
                _entities[typeName].push_back(componentName);
            }
        }
        // No file extension, is directory
        else if (std::string(ent->d_name).find(".") == std::string::npos)
        {
            // Load directory
            std::string directoryPath = std::string(archetypesDir).append("/").append(ent->d_name);
            loadDirectory(directoryPath);
        }
    }

    closedir(dir);
}

bool    EntityFactory::entityTypeExists(const std::string& type)
{
    for (auto &&entityType: _typesString)
    {
        if (entityType == type)
            return (true);
    }

    return (false);
}

Entity* EntityFactory::createEntity(eArchetype type)
{
    if ((int)type > _typesString.size() - 1)
    {
        EXCEPT(InvalidParametersException, "The entity type does not exist");
        return (nullptr);
    }

    ASSERT(_em != nullptr, "The entity manager should not be null");

    std::string typeName = _typesString[(int)type];

    return (cloneEntity(typeName));
}

void EntityFactory::bindEntityManager(EntityManager* em)
{
    _em = em;
}

const std::vector<const char*>& EntityFactory::getTypesString()
{
    return _typesString;
}

const std::list<std::string>&   EntityFactory::getComponents(const std::string& typeName)
{
    return _entities[typeName];
}

const std::string& EntityFactory::getFile(const std::string& typeName)
{
    return _entitiesFiles[typeName];
}

Entity* EntityFactory::cloneEntity(const std::string& typeName)
{
    Entity* clone = _em->createEntity();

    for (auto &&component: _entities[typeName])
    {
        sComponent* component_ = IComponentFactory::createComponent(typeName, component);
        clone->addComponent(component_);
    }

    clone->addComponent<sNameComponent>(typeName);

    return (clone);
}

void    EntityFactory::updateEntityComponent(const std::string& entityName, IComponentFactory* compFactory, sComponent* component)
{
    for (auto &&entity_: _em->getEntities())
    {
        Entity* entity = entity_.second;
        sNameComponent* name = entity->getComponent<sNameComponent>();

        if (name->value == entityName)
        {
            sComponent* entityComponent = entity->getComponent(component->getTypeInfo().hash_code());
            entityComponent->update(component);
        }
    }
}