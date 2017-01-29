/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>
#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>


#include <Game/GameStates/PauseState.hpp>
#include <Game/GameStates/PlayState.hpp>


PauseState::~PauseState() {}

void    PauseState::onEnter() {}

void    PauseState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>();
    _world.addSystem<RenderingSystem>(nullptr, _world.getSystem<ParticleSystem>()->getEmitters());
}

bool    PauseState::init()
{
    _resumeButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_RESUME);
    _howToPlayButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_HOW_TO_PLAY);
    _optionsButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_OPTIONS);
    _quitButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_QUIT);

    SoundManager::getInstance()->setVolume(0.3f);

    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    renderPreviousStates({PlayState::identifier});

    bool success = GameState::update(elapsedTime);

    // Unpause the game
    if (!handleButtons() ||
        keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        SoundManager::getInstance()->setVolume(1.0f);
        return (false);
    }

    return (success);
}

bool    PauseState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* resume = _resumeButton->getComponent<sButtonComponent>();
    sButtonComponent* howToPlay = _howToPlayButton->getComponent<sButtonComponent>();
    sButtonComponent* options = _optionsButton->getComponent<sButtonComponent>();
    sButtonComponent* quit = _quitButton->getComponent<sButtonComponent>();

    ASSERT(resume != nullptr, "\"Resume Game\" button should have sButtonComponent");
    ASSERT(howToPlay != nullptr, "\"How to Play\" button should have sButtonComponent");
    ASSERT(options != nullptr, "\"Options\" button should have sButtonComponent");
    ASSERT(quit != nullptr, "\"Quit Game\" button should have sButtonComponent");

    //  When the space bar is pressed or the mouse has clicked, do something.
    bool enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Resume Game" button
    if ((enterPressed && resume->selected) ||
        (mouseClicked && resume->hovered))
    {
        return (false);
    }

    //  "How to Play" button
    else if ((enterPressed && howToPlay->selected) ||
        (mouseClicked && howToPlay->hovered))
    {
        _gameStateManager->addState<HowToPlayState>();
    }

    //  "Options" button
    else if ((enterPressed && options->selected) ||
        (mouseClicked && options->hovered))
    {
        _gameStateManager->addState<OptionsMenuState>();
    }

    //  "Quit Game" button
    else if ((enterPressed && quit->selected) ||
        (mouseClicked && quit->hovered))
    {
        _gameStateManager->addState<ConfirmExitState>();
    }

    return (true);
}
