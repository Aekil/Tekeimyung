/*
** @Author : Mathieu Chassara
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>

#include    <Game/Scripts/GameManager.hpp>
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

    bool            _enabled = false;
    const Entity*   _tileHovered = nullptr;
    Entity*         _preview = nullptr;
    std::string     _currentChoice;
    float           _radius = 0.0f;

    //  Each archetype's string is binded to a keyboard key
    MapKeyboardEntity   _bindedEntities;

private:
    void            retrieveGameManager();

    void            bindEntitiesToInputs();
    void            checkUserInputs();
    void            triggerBuildableZone(const std::string &archetype);

    void            placePreviewedEntity();
    void            updateSpawnersPaths(const glm::ivec2& tilePos);
};

REGISTER_SCRIPT(NewBuild);