/**
* @Author   Mathieu Chassara
*/

#include <Engine/Core/GameState.hpp>

#include <Engine/Utils/EventSound.hpp>

START_GAMESTATE(HomeScreenState, "HomeScreen")
 public:
     ~HomeScreenState();

     void            onEnter() override final;
     void            setupSystems() override final;
     bool            init() override final;
     //bool            update(float elapsedTime);

private:
    Entity*         _buttonHowToPlay = nullptr;

    tEventSound*    _backgroundMenuMusic = nullptr;
    tEventSound*    _backgroundGameMusic = nullptr;

END_GAMESTATE(HomeScreenState)