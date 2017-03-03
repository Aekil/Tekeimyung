/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Engine/Systems/UISystem.hpp>

UISystem::UISystem()
{
    addDependency<sRenderComponent>();
    addDependency<sUiComponent>();
    addDependency<sTransformComponent>();
}

UISystem::~UISystem() {}

void    UISystem::update(EntityManager& em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity) {
        handleAlignment(em, entity);
    });
}

void    UISystem::onWindowResize(EntityManager &em)
{
    forEachEntity(em, [&](Entity *entity) {
        handleAlignment(em, entity, true);
    });
}

void    UISystem::handleAlignment(EntityManager& em, Entity* entity, bool forceUpdate)
{
    sUiComponent* ui = entity->getComponent<sUiComponent>();

    if (ui && (ui->needUpdate || forceUpdate))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        const glm::vec3& size = render->getModel()->getSize() * transform->getScale();
        float windowWidth = (float) GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float) GameWindow::getInstance()->getBufferHeight();

        // Horizontal alignments
        if (ui->horizontalAlignment == eHorizontalAlignment::LEFT)
        {
            float x = size.x / 2.0f;
            transform->setPos(glm::vec3(x, transform->getPos().y, transform->getPos().z));
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::MIDDLE)
        {
            float x = windowWidth / 2.0f;
            transform->setPos(glm::vec3(x, transform->getPos().y, transform->getPos().z));
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::RIGHT)
        {
            float x = windowWidth - (size.x / 2.0f);
            transform->setPos(glm::vec3(x, transform->getPos().y, transform->getPos().z));
        }

        // Vertical alignments
        if (ui->verticalAlignment == eVerticalAlignment::TOP)
        {
            float y = windowHeight - (size.y / 2.0f);
            transform->setPos(glm::vec3(transform->getPos().x, y, transform->getPos().z));
        }
        else if (ui->verticalAlignment == eVerticalAlignment::MIDDLE)
        {
            float y = windowHeight / 2.0f;
            transform->setPos(glm::vec3(transform->getPos().x, y, transform->getPos().z));
        }
        else if (ui->verticalAlignment == eVerticalAlignment::BOTTOM)
        {
            float y = size.y / 2.0f;
            transform->setPos(glm::vec3(transform->getPos().x, y, transform->getPos().z));
        }

        // Offsets
        glm::vec3 pos = transform->getPos();
        pos.x += windowWidth * ui->offset.x / 100.0f;
        pos.y += windowHeight * ui->offset.y / 100.0f;
        pos.z = -(float)ui->layer;
        transform->setPos(pos);
    }

    ui->needUpdate = false;
}
