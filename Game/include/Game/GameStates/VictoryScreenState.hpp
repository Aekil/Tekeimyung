/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/GameState.hpp>

#include <Engine/Utils/EventSound.hpp>

START_GAMESTATE(VictoryScreenState, "VictoryScreen")
 public:
     ~VictoryScreenState();

     void           onEnter() override final;
     void           setupSystems() override final;
     bool           init() override final;

private:
    Entity*         _buttonTryAgainGreen = nullptr;

    tEventSound*    _victorySound = nullptr;

    END_GAMESTATE(VictoryScreenState)