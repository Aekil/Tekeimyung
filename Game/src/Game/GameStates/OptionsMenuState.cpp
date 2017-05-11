/**
* @Author   Mathieu Chassara
*/

#include    <iostream>
#include    <Engine/Window/GameWindow.hpp>

#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Components.hh>

#include    <Game/GameStates/OptionsMenuState.hpp>
#include    <Game/GameStates/PlayState.hpp>

OptionsMenuState::~OptionsMenuState() {}

void    OptionsMenuState::onEnter() {}

void    OptionsMenuState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool            OptionsMenuState::init()
{
    //this->createOrGetButtons();
    return (true);
}

bool        OptionsMenuState::update(float elapsedTime)
{
    auto    &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    this->renderPreviousStates({PlayState::identifier});

    bool    success = GameState::update(elapsedTime);

    // Quit the state
    if (/*!handleButtons() ||*/
        keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

bool                    OptionsMenuState::handleButtons()
{
    auto                &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto                &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent*   buttonComponent = this->_buttonCurrentWindowMode->getComponent<sButtonComponent>();

    ASSERT(buttonComponent != nullptr, " The \"Current window mode\" button should have a sButtonComponent.");

    bool    enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool    mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    //  "Toggle fullscreen / windowed mode" button
    if ((enterPressed && buttonComponent->selected) ||
        (mouseClicked && buttonComponent->hovered))
    {
        GameWindow::getInstance()->toggleFullscreen();
        this->createOrGetButtons();
    }

    return (true);
}

void                    OptionsMenuState::createOrGetButtons()
{
    bool                isFullscreen = GameWindow::getInstance()->isFullscreen();
    std::string         buttonArchetype;

    buttonArchetype = (isFullscreen ? "BUTTON_TOGGLE_WINDOWED" : "BUTTON_TOGGLE_FULLSCREEN");
    if (this->_buttonCurrentWindowMode == nullptr)
        this->_buttonCurrentWindowMode = EntityFactory::createOrGetEntity(buttonArchetype);
    else
    {
        EntityManager*  em = EntityFactory::getBindedEntityManager();

        if (em != nullptr)
        {
            em->destroyEntity(this->_buttonCurrentWindowMode->handle);

            buttonArchetype = (isFullscreen ? "BUTTON_TOGGLE_WINDOWED" : "BUTTON_TOGGLE_FULLSCREEN");
            this->_buttonCurrentWindowMode = EntityFactory::createOrGetEntity(buttonArchetype);
        }
    }
}
