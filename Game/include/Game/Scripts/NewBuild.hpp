/*
** @Author : Mathieu Chassara
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>

#include    <Game/Scripts/GameManager.hpp>
#include    <Game/Scripts/Player.hpp>
#include    <Game/Scripts/GoldManager.hpp>
#include    <Game/Scripts/Tile.hpp>

typedef     std::unordered_map<Keyboard::eKey, std::string>   MapKeyboardEntity;

class       NewBuild final : public BaseScript
{
public:
    NewBuild() = default;
    ~NewBuild() = default;

    //  BaseScript  overrides
public:
    virtual void    start() override final;
    virtual void    update(float dt) override final;

public:
    bool    isEnabled() const;
    void    setEnabled(bool enabled);
    void    disableAll();

    void    setTileHovered(const Entity* tileHovered);

private:
    GameManager*    _gameManager = nullptr;
    GoldManager*    _goldManager = nullptr;
    Player*         _player = nullptr;

    bool            _enabled = false;
    const Entity*   _tileHovered = nullptr;
    Entity*         _preview = nullptr;
    std::string     _currentChoice;
    float           _radius = 0.0f;

    //  Each archetype's string is binded to a keyboard key
    MapKeyboardEntity   _bindedEntities;

    std::vector<uint32_t> _alreadyBuiltTile;

    std::map<std::string, int> _buildingPrices;

private:
    void            retrievePlayerScript();
    void            retrieveManagers();

    void            bindEntitiesToInputs();
    void            checkUserInputs();
    void            triggerBuildableZone(const std::string &archetype);

    void            placePreviewedEntity();
    void            updateSpawnersPaths(const glm::ivec2& tilePos);

    void            initPrices();
};

REGISTER_SCRIPT(NewBuild);