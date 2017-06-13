/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>
#include <ECS/World.hpp>

#include <Engine/Core/GameState.hpp>
#include <Engine/Utils/EventSound.hpp>


START_GAMESTATE(ConsoleState, "Console")
 public:
    ~ConsoleState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

private:
    void                                handleCheatCodeKillAll();
    void                                handleCheatCodeGiveMeGold();
    void                                handleCheatCodeBuildForMe();
    void                                handleCheatCode(const std::string& cheatCode);

 private:
    tEventSound*                        _backgroundGameMusic = nullptr;

    Entity*                             _console = nullptr;;
    EntityManager*                      _playStateEntityManager = nullptr;

    Entity::sHandle                     _lastBuiltBaseTurretHandle = 0;
END_GAMESTATE(ConsoleState)
