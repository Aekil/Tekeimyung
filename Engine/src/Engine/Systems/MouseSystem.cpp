/*
** @Author: Simon AMBROISE
*/

#include <Engine/Core/Components/RenderComponent.hh>
#include <Engine/Core/Components/ScriptComponent.hh>
#include <Engine/Core/Components/TransformComponent.hh>
#include <Engine/Core/Components/UiComponent.hh>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Physics/Physics.hpp>
#include <Engine/Graphics/Renderer.hpp>

#include <Engine/Systems/MouseSystem.hpp>

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


    Entity* selectedEntity = getHoveredUI(em);
    // No UI hovered, find for 3D entities
    if (!selectedEntity)
    {
        Ray ray = camera->screenPosToRay((float)cursor.getX(), (float)cursor.getY());
        Physics::raycast(ray, &selectedEntity);
    }

    Entity* previousEntity = em.getEntity(this->previousEntityHandler);

    if (selectedEntity != nullptr)
    {
        if (previousEntity != nullptr && previousEntity->handle != selectedEntity->handle)
        {
            if (previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : previousEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverExit();

            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverEnter();

            this->previousEntityHandler = selectedEntity->handle;
        }
        else if (previousEntity == nullptr)
        {
            if (selectedEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : selectedEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverEnter();

            this->previousEntityHandler = selectedEntity->handle;
        }
    }
    else
    {
        if (previousEntity != nullptr)
        {
            if (previousEntity->getComponent<sScriptComponent>() != nullptr)
                for (auto&& script : previousEntity->getComponent<sScriptComponent>()->scripts)
                    script->onHoverExit();

            this->previousEntityHandler = 0;
        }
    }
}

Entity* MouseSystem::getHoveredUI(EntityManager &em)
{
    Entity* nearestUI = nullptr;
    int nearestLayer = INT_MAX;

    std::vector<Entity*>    entities = em.getEntities();
    float       windowHeight = (float)GameWindow::getInstance()->getBufferHeight();
    auto&&      cursor = GameWindow::getInstance()->getMouse().getCursor();
    glm::vec2   cursorPos = glm::vec2(cursor.getX(), windowHeight - cursor.getY());

    for (Entity* entity : entities)
    {
        sUiComponent* ui = entity->getComponent<sUiComponent>();
        if (!ui)
        {
            continue;
        }
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (!render)
        {
            continue;
        }

        sTransformComponent*    transform = entity->getComponent<sTransformComponent>();
        const glm::vec2&        size = glm::vec2(render->getModel()->getSize() * transform->getScale());
        glm::vec2               pos = glm::vec2(transform->getPos()) - (size / 2.0f);

        // Check the mouse is in the button (2D AABB collision)
        if (render->enabled && !render->ignoreRaycast &&
            render->display &&
            ui->layer < nearestLayer &&
            Collisions::pointVSAABB2D(cursorPos, pos, size))
        {
            nearestLayer = ui->layer;
            nearestUI = entity;
        }
    }

    return (nearestUI);
}
