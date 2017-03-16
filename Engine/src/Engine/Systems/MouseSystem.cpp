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

    Ray ray = camera->screenPosToRay((float)cursor.getX(), (float)cursor.getY());
    Physics::raycast(ray, &selectedEntity);

    Entity* previousEntity = em.getEntity(this->previousEntityId);

    if (selectedEntity != nullptr)
    {
        if (previousEntity != nullptr && previousEntity->id != selectedEntity->id)
        {
            if (previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : previousEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverExit();

            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverEnter();

            this->previousEntityId = selectedEntity->id;
        }
        else if (previousEntity == nullptr)
        {
            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverEnter();

            this->previousEntityId = selectedEntity->id;
        }
    }
    else
    {
        if (previousEntity != nullptr)
        {
            if (previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : previousEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverExit();

            this->previousEntityId = 0;
        }
    }
}
