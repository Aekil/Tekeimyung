#include "json/json.h"
#include "Utils/Exception.hpp"
#include "Utils/RessourceManager.hpp"
#include "EntityFactory.hpp"
#include "ComponentFactory.hpp"
#include "dirent.h"



std::unordered_map<std::string, Entity*>  EntityFactory::_entities;
std::vector<std::string>  EntityFactory::_typesString = { ENTITIES_TYPES(GENERATE_STRING) };

EntityFactory::EntityFactory() {}

EntityFactory::~EntityFactory() {}

void EntityFactory::init()
{
    DIR* dir;
    struct dirent* ent;
    RessourceManager* ressourceManager = RessourceManager::getInstance();

    dir = opendir("ressources/archetypes");
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory");

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            Entity* entity;

            // Get entity configuration file
            std::string path = std::string("ressources/archetypes/").append(ent->d_name);
            std::string entityConf = ressourceManager->getFile(path);

            // Parse the configuration file with jsoncpp
            Json::Reader jsonReader;
            Json::Value parsed;
            if (!jsonReader.parse(entityConf, parsed))
                EXCEPT(IOException, "Cannot parse archetype .json");
            else if (!EntityFactory::entityTypeExists(parsed["name"].asString()))
                EXCEPT(InvalidParametersException, "Failed to read entity archetype: Entity type does not exist");

            entity = new Entity(-1);
            for (Json::ValueIterator it = parsed["components"].begin(); it != parsed["components"].end(); it++)
            {
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                    EXCEPT(InvalidParametersException, "Failed to read entity archetype: Component type does not exist");

                sComponent* component = IComponentFactory::createComponent(it.key().asString(), *it);
                entity->addComponent(component);
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

Entity* EntityFactory::createEntity(eArchetype& type)
{
    if ((int)type > _typesString.size() - 1)
    {
        // TODO: Assert not null
        return (nullptr);
    }
    return EntityFactory::_entities[_typesString[(int)type]];
}
