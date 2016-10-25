#include "json/json.h"
#include "Utils/Exception.hpp"
#include "Utils/RessourceManager.hpp"
#include "ComponentFactory.hpp"
#include "dirent.h"
#include "Utils/Debug.hpp"

#include "EntityFactory.hpp"


std::unordered_map<std::string, std::list<std::string> >  EntityFactory::_entities;
std::vector<std::string>  EntityFactory::_typesString = { ENTITIES_TYPES(GENERATE_STRING) };
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
            Json::Reader jsonReader;
            Json::Value parsed;
            std::string typeName;
            if (!jsonReader.parse(entityConf, parsed))
                EXCEPT(IOException, "Cannot parse archetype \"%s\"", path.c_str());

            typeName = parsed["name"].asString();

            if (!EntityFactory::entityTypeExists(typeName)) // The macro ENTITIES_TYPES did not create the type
                EXCEPT(InvalidParametersException, "Failed to read entity archetype: Entity type \"%s\" does not exist", typeName.c_str());

            // Create entity components
            for (Json::ValueIterator it = parsed["components"].begin(); it != parsed["components"].end(); it++)
            {
                std::string componentName = it.key().asString();

                // The macro COMPONENTS_TYPES did not create the type
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                    EXCEPT(InvalidParametersException, "Failed to read entity archetype: Component type \"%s\" does not exist", componentName.c_str());

                IComponentFactory::initComponent(typeName, componentName, *it);
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
