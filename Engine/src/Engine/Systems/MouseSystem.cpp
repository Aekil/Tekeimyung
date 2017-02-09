/*
** @Author: Simon AMBROISE
*/

#include <Engine/Systems/MouseSystem.hpp>

#include <Engine/Components.hh>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Physics.hpp>
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
    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return;


    Entity* selectedEntity = nullptr;

    Ray ray = camera->screenPosToRay(cursor.getX(), cursor.getY());
    Physics::raycast(ray, &selectedEntity);

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
