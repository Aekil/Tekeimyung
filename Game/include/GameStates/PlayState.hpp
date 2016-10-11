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

private:
    Map*                                _map;
    bool                                _windowImgui;
};
