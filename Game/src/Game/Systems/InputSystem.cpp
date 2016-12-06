#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Window/GameWindow.hpp>

#include <Game/Components.hh>

#include <Game/Systems/InputSystem.hpp>

InputSystem::InputSystem()
{
    addDependency<sInputComponent>();
    addDependency<sDirectionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(INPUT_SYSTEM_NAME);
}

InputSystem::~InputSystem() {}

void    InputSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;

    forEachEntity(em, [&](Entity *entity) {
        movementKeys(entity, elapsedTime);
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

void    InputSystem::movementKeys(Entity *entity, float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    sInputComponent *input = entity->getComponent<sInputComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();

    direction->value = glm::vec2(0.0f, 0.0f);
    direction->moved = true;

    if (KB_P(LEFT))
    {
        transform->rotation.y += 180.0f * elapsedTime;
        transform->rotation.y = std::fmod(transform->rotation.y, 360.0f);
        transform->needUpdate = true;
    }
    else if (KB_P(RIGHT))
    {
        transform->rotation.y -= 180.0f * elapsedTime;
        transform->rotation.y = std::fmod(transform->rotation.y, 360.0f);
        transform->needUpdate = true;
    }
    if (KB_P(UP))
    {
        float angle = glm::radians(-transform->rotation.y + 90.0f);
        direction->value.x += glm::cos(angle);
        direction->value.y += glm::sin(angle);
    }
    else if (KB_P(DOWN))
    {
        float angle = glm::radians(-transform->rotation.y + 90.0f);
        direction->value.x -= glm::cos(angle);
        direction->value.y -= glm::sin(angle);
    }
}
