/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/MenuSystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Components.hh>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/ConfirmExitState.hpp>


ConfirmExitState::~ConfirmExitState() {}

void    ConfirmExitState::onEnter()
{
     EntityFactory::bindEntityManager(_world.getEntityManager());
}

bool    ConfirmExitState::init()
{
    initPreviousStatesRender();

    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
    _world.addSystem<MenuSystem>();

    initCamera();
    initPopup();

    return (true);
}

bool    ConfirmExitState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    for (int i = (int)_previousStatesSystems.size() - 1; i >= 0; --i)
    {
        _previousStatesSystems[i]->update(*_previousStatesWorlds[i]->getEntityManager(), 0);
    }

    bool success = GameState::update(elapsedTime);

    handleButtons();

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }

    return (success);
}

void    ConfirmExitState::initCamera()
{
    auto gameWindow = GameWindow::getInstance();

    // Set camera screen
    Camera::sScreen screen;
    screen.right = (float)gameWindow->getBufferWidth();
    screen.left = 0;
    screen.top = (float)gameWindow->getBufferHeight();
    screen.bottom = 0;
    _camera.setScreen(screen);
    _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
}

void    ConfirmExitState::initPreviousStatesRender()
{
    uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();

    ASSERT(statesNb >= 1, "The ConfirmExitState should not be the first state");

    auto previousState = _gameStateManager->getStates()[statesNb - 1];
    World* previousStateWorld = &previousState->getWorld();
    System* previousStateRendersystem = previousStateWorld->getSystem<RenderingSystem>();

    _previousStatesSystems.push_back(previousStateRendersystem);
    _previousStatesWorlds.push_back(previousStateWorld);

    // Add playState render
    if (previousState->getId() == PauseState::identifier)
    {
        ASSERT(statesNb >= 2, "The Pause State should not be the first state");

        auto playState = _gameStateManager->getStates()[statesNb - 2];
        World* playStateWorld = &playState->getWorld();
        System* playStateRendersystem = playStateWorld->getSystem<RenderingSystem>();

        _previousStatesSystems.push_back(playStateRendersystem);
        _previousStatesWorlds.push_back(playStateWorld);
    }
}

void    ConfirmExitState::initPopup()
{
    float windowWidth = (float)GameWindow::getInstance()->getBufferWidth();
    float windowHeight = (float)GameWindow::getInstance()->getBufferHeight();

    _confirmPopup = EntityFactory::createEntity(eArchetype::QUIT_CONFIRM_POPUP);
    _confirmText = EntityFactory::createEntity(eArchetype::QUIT_CONFIRM_TEXT);
    _noButton = createButton(eArchetype::BUTTON_CONFIRM_NO, glm::vec2(0.0f, -70.0f));
    _yesButton = createButton(eArchetype::BUTTON_CONFIRM_YES, glm::vec2(0.0f, 20.0f));

    // Text
    sRenderComponent* textRender = _confirmText->getComponent<sRenderComponent>();
    sTransformComponent* textTransform = _confirmText->getComponent<sTransformComponent>();
    if (!textRender->_model)
        textRender->initModel();
    // Popup
    sRenderComponent* popupRender = _confirmPopup->getComponent<sRenderComponent>();
    if (!popupRender->_model)
        popupRender->initModel();

    // Yes
    sRenderComponent* yesRender = _yesButton->getComponent<sRenderComponent>();
    sTransformComponent* yesTransform = _yesButton->getComponent<sTransformComponent>();
    if (!yesRender->_model)
        yesRender->initModel();
    // No
    sRenderComponent* noRender = _noButton->getComponent<sRenderComponent>();
    sTransformComponent* noTransform = _noButton->getComponent<sTransformComponent>();
    if (!noRender->_model)
        noRender->initModel();

    /* Init popup */
    sTransformComponent* popupTransform = _confirmPopup->getComponent<sTransformComponent>();
    popupTransform->scale.x = (textRender->_model->getSize().x * textTransform->scale.x + 50.0f) / SIZE_UNIT;
    popupTransform->scale.y = (textRender->_model->getSize().y * textTransform->scale.y +\
        yesRender->_model->getSize().y * yesTransform->scale.y +\
        noRender->_model->getSize().y * noTransform->scale.y +\
        (std::abs(yesTransform->pos.y - noTransform->pos.y)))/ SIZE_UNIT;
    const glm::vec3& popupSize = popupRender->_model->getSize() * popupTransform->scale;

    // Center the popup in the window
    popupTransform->pos.x = (windowWidth / 2.0f) - (popupSize.x / 2.0f);
    popupTransform->pos.y = (windowHeight / 2.0f) - (popupSize.y / 2.0f);
    popupTransform->needUpdate = true;


    /* Init popup text */
    const glm::vec3& textSize = textRender->_model->getSize() * textTransform->scale;
    // Center the text in the popup
    textTransform->pos = popupTransform->pos;
    textTransform->pos.y += popupSize.y - textSize.y - 5.0f;
    textTransform->pos.x += (popupSize.x - textSize.x) / 2.0f;
    textTransform->needUpdate = true;
}

Entity* ConfirmExitState::createButton(eArchetype type, const glm::vec2& pos)
{
    Entity* button = EntityFactory::createEntity(type);

    sTransformComponent* transform = button->getComponent<sTransformComponent>();
    transform->pos += glm::vec3(pos, 0.0f);
    transform->needUpdate = true;

    return (button);
}

void    ConfirmExitState::handleButtons()
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    sButtonComponent* yes = _yesButton->getComponent<sButtonComponent>();
    sButtonComponent* no = _noButton->getComponent<sButtonComponent>();

    bool enterPressed = keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED;
    bool mouseClicked = mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED;

    // Yes button
    // Space bar pressed or mouse clicked
    if ((enterPressed && yes->selected) ||
        (mouseClicked && yes->hovered))
    {
        _gameStateManager->clearStates();
    }

    // No button
    // Space bar pressed or mouse clicked
    else if ((enterPressed && no->selected) ||
        (mouseClicked && no->hovered))
    {
        _gameStateManager->removeCurrentState();
    }
}
