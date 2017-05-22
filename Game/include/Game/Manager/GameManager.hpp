/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Map/Map.hpp>

#define GAME_MANAGER_TAG    "GameManager"

class GameManager final : public BaseScript
{
public:
    GameManager();
    ~GameManager() = default;

public:
    void start() override final;
    void update(float dt) override final;

    void displayMapParts(int);

//  Json serialization & ImGui edition.
public:
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

public:
    Map map;

    Map emptyMap; // Save of the first load of the map

private:
    std::string _mapName;
};

REGISTER_SCRIPT(GameManager);
