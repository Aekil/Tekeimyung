#include <iostream>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>
#include <Game/Systems/MenuSystem.hpp>
#include <Game/EntityDebugWindow.hpp>
#include <Game/EntityFactory.hpp>
#include <Game/Components.hh>
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

    _world.addSystem<RenderingSystem>(&_camera, nullptr, nullptr);
    _world.addSystem<MenuSystem>();

    EntityManager* em = _world.getEntityManager();
    addDebugWindow<EntityDebugWindow>(em, nullptr, glm::vec2(0, 80), glm::vec2(600, 350));

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
    _yesButton = createButton(eArchetype::BUTTON_CONFIRM_YES, glm::vec2(-60.0f, -10.0f));
    _noButton = createButton(eArchetype::BUTTON_CONFIRM_NO, glm::vec2(60.0f, -10.0f));

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
        noRender->_model->getSize().y * noTransform->scale.y) / SIZE_UNIT;
    const glm::vec3& popupSize = popupRender->_model->getSize() * popupTransform->scale;

    // Center the popup in the window
    popupTransform->pos.x = (windowWidth / 2.0f) - (popupSize.x / 2.0f);
    popupTransform->pos.y = (windowHeight / 2.0f) - (popupSize.y / 2.0f);
    popupTransform->pos.z = -1.0f;
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

    sButtonComponent* yes = _yesButton->getComponent<sButtonComponent>();
    sButtonComponent* no = _noButton->getComponent<sButtonComponent>();

    // Space bar pressed, handle buttons action
    if (keyboard.getStateMap()[Keyboard::eKey::SPACE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        // Yes button
        if (yes->selected)
        {
            _gameStateManager->clearStates();
        }
        // No button
        else if (no->selected)
        {
            _gameStateManager->removeCurrentState();
        }
    }
}
