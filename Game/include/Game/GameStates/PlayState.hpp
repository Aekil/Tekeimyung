/**
* @Author   Guillaume Labey
*/

#pragma once

#include <array>
#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Window/IInputEvent.hpp>
#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <Engine/EntityFactory.hpp>
#include <Game/Map.hpp>
#include <Engine/Utils/EventSound.hpp>

START_GAMESTATE(PlayState, "Game")
 public:
    ~PlayState();

    void                                onEnter();
    void                                setupSystems() override final;
    bool                                init();
    bool                                update(float elapsedTime);

    void                                initCamera();
    void                                initEntities();

private:
    void                                updateCameraInputs(float elapsedTime);
    void                                limitCameraZoom();

private:
    Map*                                _map;
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;

    Camera                              _camera;
    tEventSound*                        _backgroundMusic = nullptr;
END_GAMESTATE(PlayState)
