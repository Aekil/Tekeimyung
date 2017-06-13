/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>

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
    static void                         handleCheatCodeKillAll(std::shared_ptr<GameState> playState);
    static void                         handleCheatCodeGiveMeGold(std::shared_ptr<GameState> playState);
    static void                         handleCheatCodeBuildForMe(std::shared_ptr<GameState> playState);
    static void                         handleCheatCodeAegis(std::shared_ptr<GameState> playState);
    static void                         handleCheatCode(std::shared_ptr<GameState> playState, const std::string& cheatCode);

 private:
    tEventSound*                        _backgroundGameMusic = nullptr;

    Entity*                             _console = nullptr;

    static Entity::sHandle              _lastBuiltBaseTurretHandle;
END_GAMESTATE(ConsoleState)
