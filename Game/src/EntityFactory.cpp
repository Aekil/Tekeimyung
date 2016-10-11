#include "json/json.h"
#include "Utils/Exception.hpp"
#include "Utils/RessourceManager.hpp"
#include "EntityFactory.hpp"
#include "ComponentFactory.hpp"
#include "dirent.h"



std::unordered_map<std::string, std::list<std::string> >  EntityFactory::_entities;
std::vector<std::string>  EntityFactory::_typesString = { ENTITIES_TYPES(GENERATE_STRING) };
EntityManager*  EntityFactory::_em = nullptr;

EntityFactory::EntityFactory() {}

EntityFactory::~EntityFactory() {}

void EntityFactory::init(const std::string& archetypesDir)
{
    DIR* dir;
    struct dirent* ent;
    RessourceManager* ressourceManager = RessourceManager::getInstance();

    dir = opendir(archetypesDir.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory");

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            // Get entity configuration file
            std::string path = std::string(archetypesDir).append("/").append(ent->d_name);
            std::string entityConf = ressourceManager->getFile(path);

            // Parse the configuration file with jsoncpp
            Json::Reader jsonReader;
            Json::Value parsed;
            std::string typeName;
            if (!jsonReader.parse(entityConf, parsed))
                EXCEPT(IOException, "Cannot parse archetype .json");

            typeName = parsed["name"].asString();

            if (!EntityFactory::entityTypeExists(typeName)) // The macro ENTITIES_TYPES did not create the type
                EXCEPT(InvalidParametersException, "Failed to read entity archetype: Entity type does not exist");

            // Create entity components
            for (Json::ValueIterator it = parsed["components"].begin(); it != parsed["components"].end(); it++)
            {
                std::string componentName = it.key().asString();

                // The macro COMPONENTS_TYPES did not create the type
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                    EXCEPT(InvalidParametersException, "Failed to read entity archetype: Component type does not exist");

                IComponentFactory::initComponent(typeName, componentName, *it);
                _entities[typeName].push_back(componentName);
            }
        }
    }
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
        // TODO: Assert not null
        return (nullptr);
    }

    // TODO: Assert _em not null

    std::string typeName = _typesString[(int)type];

    return (cloneEntity(typeName));
}

void EntityFactory::bindEntityManager(EntityManager* em)
{
    _em = em;
}

Entity* EntityFactory::cloneEntity(const std::string& typeName)
{
    Entity* clone = _em->createEntity();

    for (auto &&component: _entities[typeName])
    {
        sComponent* component_ = IComponentFactory::createComponent(typeName, component);
        clone->addComponent(component_);
    }

    return (clone);
}
