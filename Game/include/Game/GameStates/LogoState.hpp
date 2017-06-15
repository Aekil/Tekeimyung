/**
* @Author   Simon Ambroise
*/

#include    <Engine/Core/GameState.hpp>

#include <Engine/Utils/EventSound.hpp>

START_GAMESTATE(LogoState, "Logo")
 public:
     ~LogoState();

     void           onEnter() override final;
     void           setupSystems() override final;
     bool           init() override final;
     bool           update(float elapsedTime) override final;

private:
    Entity*         _digipenLogo = nullptr;
    float           _elapsedTime = 0.0f;

    END_GAMESTATE(LogoState)