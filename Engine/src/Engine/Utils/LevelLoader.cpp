/**
* @Author   Guillaume Labey
*/


#include <Engine/ComponentFactory.hpp>
#include <dirent.h> // This include has to be called after "ComponentFactory.hpp"

#include <Engine/BasicState.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/Logger.hpp>

std::unique_ptr<LevelLoader>    LevelLoader::_instance = nullptr;

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader()
{
    for (auto level: _levels)
    {
        delete[] level;
    }
}

LevelLoader*    LevelLoader::getInstance()
{
    if (!_instance)
    {
        _instance = std::make_unique<LevelLoader>();
    }
    return (_instance.get());
}

const std::vector<const char*>& LevelLoader::getLevels() const
{
    return (_levels);
}

std::vector<const char*>& LevelLoader::getLevels()
{
    return (_levels);
}

void    LevelLoader::addLevel(const std::string& levelName)
{
    char *str = new char [levelName.size() + 1];
    strcpy_s(str, levelName.size() + 1, levelName.c_str());
    _levels.push_back(str);
}

bool    LevelLoader::hasLevel(const std::string& levelName)
{
    return (std::find(_levels.cbegin(), _levels.cend(), levelName) != _levels.cend());
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

        // Reverse animations
        EntityFactory::reverseAnimations((Entity*)entity);

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

        entityJson.setString("tag", entity->getTag());

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
        std::string entityTag = entityJson.getString("tag", "");

        if (entityTypeName.size() == 0)
            EXCEPT(InvalidParametersException, "Failed to load level \"%s\": missing entity type name", levelName.c_str());

        entity->setTag(entityTag);

        for (Json::ValueIterator it = components.begin(); it != components.end(); it++)
        {
            JsonValue componentJson(*it);
            std::string componentName = it.key().asString();
            addComponentToEntity(entity, entityTypeName, componentJson, componentName);
        }

        // Init animation
        EntityFactory::initAnimations(entity);
        em->notifyEntityCreated(entity);
    }
}

std::shared_ptr<GameState>  LevelLoader::createLevelState(const std::string& levelName, GameStateManager* gameStateManager)
{
    if (levelName.size() == 0)
    {
        return (std::make_shared<BasicState>(gameStateManager));
    }

    for (auto& state: _loadedStates)
    {
        if (state->getLevelFile() == levelName)
        {
            return (state->create(gameStateManager));
        }
    }

    std::shared_ptr<GameState> gameState = std::make_shared<BasicState>(gameStateManager);
    gameState->setLevelFile(levelName);
    return (gameState);
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
        if (ResourceManager::getFileExtension(ent->d_name) == "json")
        {
            std::string fileName(ent->d_name);
            size_t extensionFind = fileName.find_last_of(".");

            // Add the level without the .json
            addLevel(fileName.substr(0, extensionFind));
        }
    }

    closedir(dir);
}
