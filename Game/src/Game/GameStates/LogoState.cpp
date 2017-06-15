/**
* @Author   Simon Ambroise
*/

#include    <Engine/Window/Keyboard.hpp>
#include    <Engine/Window/GameWindow.hpp>
#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Systems/ButtonSystem.hpp>
#include    <Engine/Systems/RenderingSystem.hpp>
#include    <Engine/Systems/UISystem.hpp>

#include    <Game/GameStates/LogoState.hpp>
#include    <Game/GameStates/HomeScreenState.hpp>

LogoState::~LogoState() {}

void    LogoState::onEnter()
{
    sRenderComponent*   renderComponent = this->_digipenLogo->getComponent<sRenderComponent>();

    renderComponent->_animator.play("fading_logo", false);
}

void    LogoState::setupSystems()
{
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());
}

bool    LogoState::init()
{
    this->_digipenLogo = EntityFactory::createOrGetEntity("DIGIPEN_LOGO");
    return (true);
}

bool    LogoState::update(float elapsedTime)
{
    this->_elapsedTime += elapsedTime;

    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED
        || keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED
        || keyboard.getStateMap()[Keyboard::eKey::KP_ENTER] == Keyboard::eKeyState::KEY_PRESSED
        || keyboard.getStateMap()[Keyboard::eKey::SPACE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        this->_gameStateManager->removeCurrentState();
        this->_gameStateManager->addState<HomeScreenState>();
    }
    if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1) || mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_2))
    {
        this->_gameStateManager->removeCurrentState();
        this->_gameStateManager->addState<HomeScreenState>();
    }

    if (this->_elapsedTime >= 4.1f)
    {
        this->_gameStateManager->removeCurrentState();
        this->_gameStateManager->addState<HomeScreenState>();
    }

    return (GameState::update(elapsedTime));
}
