/*
** @Author: Simon AMBROISE
*/

#include <Engine/Systems/MouseSystem.hpp>

#include <Engine/Components.hh>
#include <Engine/Physics/Physics.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Graphics/Renderer.hpp>

MouseSystem::MouseSystem() {}

void MouseSystem::update(EntityManager &em, float elapsedTime)
{
    this->hoverEntity(em);
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

    // Calculate vector from near plane to far plane
    glm::vec3 rayDir = farPoint - nearPoint;

    Entity* selectedEntity = nullptr;

    Physics::raycast(nearPoint, rayDir, &selectedEntity);

    if (selectedEntity != nullptr)
    {
        if (this->previousEntity != nullptr && this->previousEntity->id != selectedEntity->id)
        {
            if (this->previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : this->previousEntity->getComponent<sScriptComponent>()->scriptInstances)
                    script->OnHoverExit();

            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scriptInstances)
                    script->OnHoverEnter();

            this->previousEntity = selectedEntity;
        }
        else if (this->previousEntity == nullptr)
        {
            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scriptInstances)
                    script->OnHoverEnter();

            this->previousEntity = selectedEntity;
        }
    }
    else
    {
        if (this->previousEntity != nullptr)
        {
            if (this->previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : this->previousEntity->getComponent<sScriptComponent>()->scriptInstances)
                    script->OnHoverExit();

            this->previousEntity = nullptr;
        }
    }
}
