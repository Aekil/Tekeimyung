#pragma once

#include <array>
#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Window/IInputEvent.hpp>
#include <Engine/Core/GameState.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>
#include <Game/Utils/EventSound.hpp>

START_GAMESTATE(PlayState)
 public:
    virtual ~PlayState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

    void                                initCamera();
    void                                initEntities();
    void                                addSystems();

private:
    void                                updateCameraInputs(float elapsedTime);

private:
    Map*                                _map;
    std::pair <Keyboard::eKey, IInputEvent *>    _pair;

    Camera                              _camera;
    tEventSound*                        _backgroundMusic = nullptr;
END_GAMESTATE(PlayState)
