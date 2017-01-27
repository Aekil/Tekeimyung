/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/MenuSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>
#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>


#include <Game/GameStates/PauseState.hpp>


PauseState::~PauseState() {}

void    PauseState::onEnter() {}

bool    PauseState::init()
{
    uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();

    ASSERT(statesNb >= 1, "The pause State should not be the first state");

    auto playState = _gameStateManager->getStates()[statesNb - 1];

    _playStateWorld = &playState->getWorld();
    _playStateRendersystem = _playStateWorld->getSystem<RenderingSystem>();

    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();

    _resumeButton = createButton(eArchetype::BUTTON_RESUME, glm::vec2(0.0f, 240.0f));
    _howToPlayButton = createButton(eArchetype::BUTTON_HOW_TO_PLAY, glm::vec2(0.0f, 160.0f));
    _optionsButton = createButton(eArchetype::BUTTON_OPTIONS, glm::vec2(0.0f, 80.0f));
    _quitButton = createButton(eArchetype::BUTTON_QUIT, glm::vec2(0.0f, 0.0f));

    SoundManager::getInstance()->setVolume(0.3f);
    return (true);
}

bool    PauseState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the play game state
    if (_playStateRendersystem)
        _playStateRendersystem->update(*_playStateWorld->getEntityManager(), 0);

    bool success = GameState::update(elapsedTime);

    handleButtons();

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        SoundManager::getInstance()->setVolume(1.0f);
        return (false);
    }

    return (success);
}

Entity* PauseState::createButton(eArchetype type, const glm::vec2& pos)
{
    Entity* button = EntityFactory::createEntity(type);

    sTransformComponent* transform = button->getComponent<sTransformComponent>();
    transform->pos += glm::vec3(pos, 0.0f);
    transform->needUpdate = true;

    return (button);
}

void    PauseState::handleButtons()
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
        _gameStateManager->removeCurrentState();
        SoundManager::getInstance()->setVolume(1.0f);
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
}
