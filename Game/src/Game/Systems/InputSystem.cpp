#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/Timer.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Components.hh>

#include <Game/Systems/InputSystem.hpp>

InputSystem::InputSystem()
{
    addDependency<sInputComponent>();
    addDependency<sDirectionComponent>();
}

InputSystem::~InputSystem() {}

void    InputSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    //auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    //auto &&mouse = GameWindow::getInstance()->getMouse();

    forEachEntity(em, [&](Entity *entity) {
        movementKeys(entity);
    });
    LOG_INFO("Input system : %f secondes", timer.getElapsedTime());
}

void    InputSystem::movementKeys(Entity *entity)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    sInputComponent *input = entity->getComponent<sInputComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

    direction->value = glm::vec2(0.0f, 0.0f);
    direction->moved = true;

    if (KB_P(LEFT) && KB_P(UP) && KB_P(RIGHT) && KB_P(DOWN))
    {
        // No movement for the 4 directional keys pressed at the same time
        direction->orientation = eOrientation::S;
    }
    else if (KB_ONLY_LEFT || (KB_P(LEFT) && KB_P(UP) && KB_P(DOWN)))
    {
        direction->orientation = eOrientation::W;
        direction->value.x -= direction->speed;
        direction->value.y += direction->speed;
    }
    else if (KB_ONLY_UP || (KB_P(LEFT) && KB_P(UP) && KB_P(RIGHT)))
    {
        direction->orientation = eOrientation::N;
        direction->value.x -= direction->speed;
        direction->value.y -= direction->speed;
    }
    else if (KB_ONLY_RIGHT || (KB_P(RIGHT) && KB_P(UP) && KB_P(DOWN)))
    {
        direction->orientation = eOrientation::E;
        direction->value.x += direction->speed;
        direction->value.y -= direction->speed;
    }
    else if (KB_ONLY_DOWN || (KB_P(DOWN) && KB_P(LEFT) && KB_P(RIGHT)))
    {
        direction->orientation = eOrientation::S;
        direction->value.x += direction->speed;
        direction->value.y += direction->speed;
    }
    else if (KB_P(LEFT) && KB_P(UP))
    {
        direction->orientation = eOrientation::NW;
        direction->value.x -= direction->speed;
    }
    else if (KB_P(LEFT) && KB_P(DOWN))
    {
        direction->orientation = eOrientation::SW;
        direction->value.y += direction->speed;
    }
    else if (KB_P(RIGHT) && KB_P(UP))
    {
        direction->orientation = eOrientation::NE;
        direction->value.y -= direction->speed;
    }
    else if (KB_P(RIGHT) && KB_P(DOWN))
    {
        direction->orientation = eOrientation::SE;
        direction->value.x += direction->speed;
    }
    else
    {
        direction->moved = false;
    }
}