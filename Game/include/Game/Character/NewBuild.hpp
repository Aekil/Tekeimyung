/*
** @Author : Mathieu Chassara
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>
#include    <Engine/Utils/EventSound.hpp>

#include    <Game/Building/Tile.hpp>
#include    <Game/Character/Player.hpp>
#include    <Game/Manager/GameManager.hpp>
#include    <Game/Manager/GoldManager.hpp>

#define BUILD_ITEMS(PROCESS)                                                    \
        PROCESS(Keyboard::eKey::KEY_1, "TILE_BASE_TURRET", BUILD_BASE_TOWER)    \
        PROCESS(Keyboard::eKey::KEY_2, "TILE_WALL", BUILD_WALL)                 \
        PROCESS(Keyboard::eKey::KEY_3, "TOWER_FIRE", BUILD_TOWER)

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

    tEventSound*    _buildSound = nullptr;

    std::vector<Entity::sHandle> _alreadyBuiltTile;

    std::map<std::string, int> _buildingPrices;

private:
    void            retrievePlayerScript();
    void            retrieveManagers();

    void            checkUserInputs();
    void            triggerBuildableZone(const std::string &archetype);

    void            placePreviewedEntity();
    void            updateSpawnersPaths(const glm::ivec2& tilePos);

    void            initPrices();
    void            updatePreview();
    void            setSelectedItem(bool);
};

REGISTER_SCRIPT(NewBuild);
