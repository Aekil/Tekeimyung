/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/GameState.hpp>

START_GAMESTATE(HomeScreenState, "HomeScreen")
 public:
     ~HomeScreenState();

     void            onEnter() override final;
     void            setupSystems() override final;
     bool            init() override final;

private:
    Entity*         _playGameButton = nullptr;

    void            createPlayGameButton();
END_GAMESTATE(HomeScreenState)