#pragma once

#include <array>
#include "Core/GameState.hpp"
#include "Entity.hpp"

typedef std::array<std::array<Entity*, 10>, 10> Layer;

class PlayState: public GameState
{
 public:
    PlayState();
    virtual ~PlayState();

    virtual bool                        init();

private:
    std::vector<Layer>                  _map;
};