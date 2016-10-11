#pragma once

#include <array>
#include <memory>
#include "Core/GameState.hpp"
#include "Core/Map.hpp"
#include "Entity.hpp"

class PlayState: public GameState
{
 public:
    PlayState();
    virtual ~PlayState();

    virtual bool                        init();
    virtual bool                        update(float elapsedTime);
    void                                createEntity(const glm::vec3& pos);

private:
    Map*                                _map;
    bool                                _windowImgui;
};
