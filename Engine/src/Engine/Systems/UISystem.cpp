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

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(UI_SYSTEM_NAME);
}

UISystem::~UISystem() {}

void    UISystem::update(EntityManager& em, float elapsedTime)
{
    Timer       timer;

    forEachEntity(em, [&](Entity *entity) {
        handleAlignment(em, entity);
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), (uint32_t)_entities.size());
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

        const glm::vec3& size = render->getModel()->getSize() * transform->scale;
        float windowWidth = (float) GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float) GameWindow::getInstance()->getBufferHeight();

        // Horizontal alignments
        if (ui->horizontalAlignment == eHorizontalAlignment::LEFT)
        {
            transform->pos.x = 0;
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::MIDDLE)
        {
            transform->pos.x = (windowWidth / 2.0f) - (size.x / 2.0f);
        }
        else if (ui->horizontalAlignment == eHorizontalAlignment::RIGHT)
        {
            transform->pos.x = windowWidth - size.x;
        }

        // Vertical alignments
        if (ui->verticalAlignment == eVerticalAlignment::TOP)
        {
            transform->pos.y = windowHeight - size.y;
        }
        else if (ui->verticalAlignment == eVerticalAlignment::MIDDLE)
        {
            transform->pos.y = (windowHeight / 2.0f) - (size.y / 2.0f);
        }
        else if (ui->verticalAlignment == eVerticalAlignment::BOTTOM)
        {
            transform->pos.y = 0;
        }

        // Offsets
        transform->pos.x += windowWidth * ui->offset.x / 100.0f;
        transform->pos.y += windowHeight * ui->offset.y / 100.0f;
        transform->pos.z = 0;
        transform->needUpdate = true;
    }

    ui->needUpdate = false;
}
