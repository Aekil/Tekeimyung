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

private:
    Map*                                _map;
};
