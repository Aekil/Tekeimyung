/**
* @Author   Guillaume Labey
*/


#include <Engine/ComponentFactory.hpp>
#include <dirent.h> // This include has to be called after "ComponentFactory.hpp"

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/Logger.hpp>

std::unique_ptr<LevelLoader>    LevelLoader::_instance = nullptr;

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

LevelLoader*    LevelLoader::getInstance()
{
    if (!_instance)
    {
        _instance = std::make_unique<LevelLoader>();
    }
    return (_instance.get());
}

const std::vector<std::string>& LevelLoader::getLevels() const
{
    return (_levels);
}

std::vector<std::string>& LevelLoader::getLevels()
{
    return (_levels);
}

const std::vector<std::unique_ptr<GameStateFactory> >& LevelLoader::getLoadedStates()
{
    return (_loadedStates);
}

void    LevelLoader::save(const std::string& levelName, const std::unordered_map<uint32_t, Entity*>& entities)
{
    JsonWriter jsonWriter;
    JsonValue json;
    std::vector<JsonValue> entitiesJson;

    json.setString("name", levelName);

    for (const auto& entityPair: entities)
    {
        JsonValue entityJson;
        JsonValue entityComponentsJson;
        const Entity* entity = entityPair.second;
        const auto& components = entity->getComponents();

        for (const auto& component: components)
        {
            JsonValue componentJson;
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
            IComponentFactory::getFactory(componentName)->saveToJson("", component, &componentJson);

            // Save entity component
            entityComponentsJson.setValue(componentName, componentJson);

            // Save entity type name
            if (component->id == sNameComponent::identifier)
            {
                entityJson.setString("name", static_cast<sNameComponent*>(component)->value);
            }
        }

        // Save entity components
        entityJson.setValue("components", entityComponentsJson);
        entitiesJson.push_back(entityJson);
    }

    // Save entities
    json.setValueVec("entities", entitiesJson);

    jsonWriter.write(std::string(LEVELS_DIRECTORY) + levelName + ".json", json);
    LOG_INFO("The level %s has been saved", levelName.c_str());
}

void    LevelLoader::load(const std::string& levelName, EntityManager* em)
{
    JsonReader jsonReader;
    JsonValue parsed;

    if (!jsonReader.parse(std::string(LEVELS_DIRECTORY) + levelName + ".json", parsed))
        EXCEPT(IOException, "Cannot load level \"%s\"", levelName.c_str());

    auto &&entities = parsed.get("entities", {}).get();
    for (Json::ValueIterator entityIt = entities.begin(); entityIt != entities.end(); entityIt++)
    {
        Entity* entity = em->createEntity();
        JsonValue entityJson(*entityIt);
        auto &&components = entityJson.get("components", {}).get();
        std::string entityTypeName = entityJson.getString("name", "");

        if (entityTypeName.size() == 0)
            EXCEPT(InvalidParametersException, "Failed to load level \"%s\": missing entity type name", levelName.c_str());

        for (Json::ValueIterator it = components.begin(); it != components.end(); it++)
        {
            JsonValue componentJson(*it);
            std::string componentName = it.key().asString();
            addComponentToEntity(entity, entityTypeName, componentJson, componentName);
        }
    }
}

void LevelLoader::addComponentToEntity(Entity* entity, const std::string& entityTypeName, const JsonValue& componentJson, const std::string& componentName)
{
    auto componentFactory = IComponentFactory::getFactory(componentName);
    sComponent* component = componentFactory->loadFromJson(entityTypeName, componentJson);

    entity->addComponent(component);
}

void    LevelLoader::loadDirectory(const std::string& directory)
{
    DIR* dir;
    struct dirent* ent;

    dir = opendir(directory.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open levels directory \"%s\"", directory.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            std::string fileName(ent->d_name);
            size_t extensionFind = fileName.find_last_of(".");

            // Add the level without the .json
            _levels.push_back(fileName.substr(0, extensionFind));
        }
    }

    closedir(dir);
}
