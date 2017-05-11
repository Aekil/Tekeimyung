/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Debug/Logger.hpp>

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

bool    UISystem::init()
{
    onWindowResize(*EntityFactory::getBindedEntityManager());
    return (true);
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
    sTextComponent* textComp = entity->getComponent<sTextComponent>();

    if (ui && (ui->needUpdate || forceUpdate))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        glm::vec3 size = render->getModel()->getSize();
        float windowWidth = (float) GameWindow::getInstance()->getBufferWidth();
        float windowHeight = (float) GameWindow::getInstance()->getBufferHeight();

        // Scale
        {
            if (ui->percentageSize)
            {
                glm::vec3 scale = transform->getScale();
                scale.x = (windowWidth * ui->size.x) / size.x;
                scale.y = (windowHeight * ui->size.y) / size.y;
                transform->setScale(scale);
            }
        }

        size *= transform->getScale();

        // Horizontal alignments
        {
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
        }

        // Vertical alignments
        {
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
        }

        // Offsets
        {
            glm::vec3 pos = transform->getPos();
            pos.x += windowWidth * ui->offset.x / 100.0f;
            pos.y += windowHeight * ui->offset.y / 100.0f;
            pos.z = 0.0f;
            transform->setPos(pos);
        }

        // Text
        {
            if (textComp)
            {
                textComp->text.isDirty(false);
                alignText(textComp, size);
            }
        }
    }
    else if (textComp && textComp->text.isDirty())
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        glm::vec3 size = render->getModel()->getSize();
        size *= transform->getScale();

        textComp->text.isDirty(false);
        alignText(textComp, size);
    }

    ui->needUpdate = false;
}

void    UISystem::alignText(sTextComponent* textComp, const glm::vec3& uiSize)
{
    // Horizontal alignments
    {
        if (textComp->horizontalAlignment == eHorizontalAlignment::LEFT)
        {
            textComp->alignmentOffset.x = -uiSize.x / 2.0f;
        }
        else if (textComp->horizontalAlignment == eHorizontalAlignment::MIDDLE)
        {
            textComp->alignmentOffset.x = -textComp->text.getSize().x / 2.0f;
        }
        else if (textComp->horizontalAlignment == eHorizontalAlignment::RIGHT)
        {
            textComp->alignmentOffset.x = (uiSize.x / 2.0f) - textComp->text.getSize().x;
        }
    }

    // Vertical alignments
    {
        if (textComp->verticalAlignment == eVerticalAlignment::TOP)
        {
            textComp->alignmentOffset.y = uiSize.y / 2.0f;
        }
        else if (textComp->verticalAlignment == eVerticalAlignment::MIDDLE)
        {
            textComp->alignmentOffset.y = textComp->text.getSize().y / 2.0f;
        }
        else if (textComp->verticalAlignment == eVerticalAlignment::BOTTOM)
        {
            textComp->alignmentOffset.y = (-uiSize.y / 2.0f) + textComp->text.getSize().y;
        }
    }

    textComp->alignmentOffset.x += textComp->offset.x;
    textComp->alignmentOffset.y += textComp->offset.y;
}
