/*
** @Author: Simon AMBROISE
*/

#include <Engine/Systems/MouseSystem.hpp>

#include <Engine/Components.hh>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Graphics/Renderer.hpp>

MouseSystem::MouseSystem()
{
    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(MOUSE_SYSTEM_NAME);
}

void MouseSystem::update(EntityManager &em, float elapsedTime)
{
    Timer   timer;
    uint32_t nbEntities = 0;

    this->hoverEntity(em);

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

void MouseSystem::hoverEntity(EntityManager& em)
{
    auto gameWindow = GameWindow::getInstance();
    Mouse& mouse = gameWindow->getMouse();
    Cursor& cursor = mouse.getCursor();

    glm::vec3 nearScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 0.0f);
    glm::vec3 farScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 1.0f);

    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return;

    // Unproject 2D points to get 3D points
    // Get 3D point on near plane
    glm::vec3 nearPoint = glm::unProject(nearScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());
    // Get 3D point on far plane
    glm::vec3 farPoint = glm::unProject(farScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());

    Entity* selectedEntity = nullptr;
    float selectedEntityDist = 0.0f;

    for (auto it : em.getEntities())
    {
        Entity* entity = it.second;

        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->_model)
            continue;

        // The selectable zone of an entity is a box
        glm::vec3 boxPos;
        glm::vec3 boxCenter;
        glm::vec3 boxSize;

        // Model box collider position
        boxPos = glm::vec3(render->_model->getMin().x, render->_model->getMin().y, render->_model->getMin().z);

        // Model box collider size
        boxSize.x = render->_model->getSize().x;
        boxSize.y = render->_model->getSize().y;
        boxSize.z = render->_model->getSize().z;

        // Model box collider center
        boxCenter = boxPos;
        boxCenter.x += (boxSize.x / 2.0f);
        boxCenter.y += (boxSize.y / 2.0f);
        boxCenter.z += (boxSize.z / 2.0f);

        // Convert box collider pos and center to world position
        boxPos += transform->pos;
        boxCenter += transform->pos;

        // Calculate vector from near plane to far plane
        glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

        // Calculate the length of this vector
        float rayLength = glm::distance(nearPoint, farPoint);

        // Calculate vector from near plane to object
        glm::vec3 objDir = boxCenter - nearPoint;

        // Project objDir onto rayDir vector
        // It gives us a scalar that is the length of objDir onto rayDir
        float scalar = glm::dot(objDir, rayDir);


        // Find closest point
        glm::vec3 closestPoint;

        // The projected objDir length is less or equal to 0 (the projected vector objDir points backward)
        // (So the point is the nearPoint or before the nearPoint)
        if (scalar <= 0.0f)
            closestPoint = nearPoint;
        // The projected objDir length is greater or equal than the vector that goes from near plane to far plane
        // (So the point is the farPoint or behind the farPoint)
        else if (scalar >= rayLength)
            closestPoint = farPoint;
        // The point is between the near plane and the far plane
        else
            closestPoint = nearPoint + (rayDir * scalar);

        // Check if the point is in the box of the entity selection
        if (Collisions::pointVSAABB(closestPoint, boxPos, boxSize))
        {
            // Calculate point distance
            float closestPointDist = glm::distance(nearPoint, closestPoint);

            // The ray can select multiple entities, we need to store the closest one
            if (selectedEntity == nullptr || closestPointDist <= selectedEntityDist)
            {
                selectedEntityDist = closestPointDist;
                selectedEntity = entity;
            }
        }
    }

    if (selectedEntity != nullptr)
    {
        if (this->previousEntity != nullptr && this->previousEntity->id != selectedEntity->id)
        {
            if (this->previousEntity->getComponent<sScriptComponent>() != nullptr)
                this->previousEntity->getComponent<sScriptComponent>()->scriptInstance->OnHoverExit();

            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                selectedEntity->getComponent<sScriptComponent>()->scriptInstance->OnHoverEnter();

            this->previousEntity = selectedEntity;
        }
        else if (this->previousEntity == nullptr)
        {
            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                selectedEntity->getComponent<sScriptComponent>()->scriptInstance->OnHoverEnter();

            this->previousEntity = selectedEntity;
        }
    }
    else
    {
        if (this->previousEntity != nullptr)
        {
            if (this->previousEntity->getComponent<sScriptComponent>() != nullptr)
                this->previousEntity->getComponent<sScriptComponent>()->scriptInstance->OnHoverExit();

            this->previousEntity = nullptr;
        }
    }
}