/**
* @Author   Guillaume Labey
*/

#include <Engine/ComponentFactory.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/Logger.hpp>

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

bool    LevelLoader::save(const std::string& levelName, const std::unordered_map<uint32_t, Entity*>& entities)
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

            entityComponentsJson.setValue(componentName, componentJson);
        }

        entityJson.setValue("components", entityComponentsJson);
        entitiesJson.push_back(entityJson);
    }

    json.setValueVec("entities", entitiesJson);

    jsonWriter.write(std::string("resources/levels/") + levelName + ".json", json);
    LOG_INFO("The level %s has been saved", levelName.c_str());
    return (true);
}
