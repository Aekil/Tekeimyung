#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>

#include <Game/Map/MapLoader.hpp>
#include <Game/Manager/GameManager.hpp>

REGISTER_SCRIPT(GameManager);

GameManager::GameManager() {}

void GameManager::start()
{
    if (!MapLoader::load(_mapName, map))
    {
        EXCEPT(InternalErrorException, "Failed to load map %s", _mapName);
    }

    emptyMap = map;
    for (int x = 0; x < emptyMap.getWidth(); ++x)
    {
        for (int z = 0; z < emptyMap.getHeight(); ++z)
        {
            if (emptyMap[x][z] % LAYER_NUMBER == 5)
            {
                emptyMap[x][z] = 1;
            }
        }
    }
}

void GameManager::update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    const auto& projectiles = em->getEntitiesByTag("Projectile");
    for (auto &projectile : projectiles)
    {
        sTransformComponent* transform = projectile->getComponent<sTransformComponent>();
        if (transform->getPos().x < 0 || transform->getPos().z < 0 ||
            transform->getPos().x > map.getWidth() * 25 ||
            transform->getPos().z > map.getHeight() * 25)
        {
            Destroy(projectile);
        }
    }
}

void GameManager::displayMapParts(int part)
{
    auto& mapParts = map.getMapParts()[part];

    for (auto& mapPart : mapParts)
    {
        auto render = mapPart->getComponent<sRenderComponent>();
        render->enabled = true;
        auto script = mapPart->getComponent<sScriptComponent>();
        script->enabled = true;
    }
}


bool        GameManager::updateEditor()
{
    bool        changed = false;
    ImGui::Text("Map name");

    std::vector<char> animationNameVec(this->_mapName.cbegin(), this->_mapName.cend());
    animationNameVec.push_back(0);
    animationNameVec.resize(64);

    if (ImGui::InputText("Name", animationNameVec.data(), animationNameVec.size()))
    {
        this->_mapName = animationNameVec.data();
        changed |= true;
    }

    return (changed);
}

JsonValue   GameManager::saveToJson()
{
    JsonValue               gameManagerJson;

    gameManagerJson.setString("map_name", this->_mapName);

    return (gameManagerJson);
}

void        GameManager::loadFromJson(const JsonValue& json)
{
    this->_mapName = json.getString("map_name", "");
}
