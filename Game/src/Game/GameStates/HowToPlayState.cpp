/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Window/GameWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/MenuSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>

#include <Game/GameStates/HowToPlayState.hpp>


HowToPlayState::~HowToPlayState() {}

void    HowToPlayState::onEnter() {}

void    HowToPlayState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();
}

bool    HowToPlayState::init()
{
    _returnButton = EntityFactory::createOrGetEntity(eArchetype::BUTTON_RETURN);

    return (true);
}

bool    HowToPlayState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    bool success = GameState::update(elapsedTime);

    // Quit the state
    if (!handleButtons() ||
        keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

bool    HowToPlayState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* returnButton = _returnButton->getComponent<sButtonComponent>();

    bool spacebarPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Return" button
    if ((spacebarPressed && returnButton->selected) ||
        (mouseClicked && returnButton->hovered))
    {
        return (false);
    }

    return (true);
}
