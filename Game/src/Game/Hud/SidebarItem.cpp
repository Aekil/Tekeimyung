/**
* @Author   Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Game/Hud/SidebarItem.hpp>

REGISTER_SCRIPT(SidebarItem);

void SidebarItem::start()
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* infoEntity = em->getEntityByTag("SidebarItemInfo");

    if (!infoEntity)
    {
        LOG_ERROR("Can't find an entity with tag SidebarItemInfo");
        return;
    }

    _infoRender = infoEntity->getComponent<sRenderComponent>();
    _infoText = infoEntity->getComponent<sTextComponent>();
}

void SidebarItem::update(float dt)
{
}

void SidebarItem::onHoverEnter()
{
    displayTextInfo(true);
}

void SidebarItem::onHoverExit()
{
    displayTextInfo(false);
}

void    SidebarItem::displayTextInfo(bool display)
{
    if (!_infoRender || !_infoText)
    {
        return;
    }

    _infoRender->enabled = display;
    // TODO: remove display from sRenderComponent and only used enabled
    _infoRender->display = display;
    _infoText->text.setContent(_description);
}

void    SidebarItem::setDescription(const std::string& description)
{
    _description = description;
}


void    SidebarItem::switchColor(bool selected)
{
    auto renderComponent = this->_border->getComponent<sRenderComponent>();
    auto& meshInstances = renderComponent->getModelInstance()->getMeshsInstances();

    for (auto& meshInstance : meshInstances)
    {
        if (selected)
        {
            meshInstance->getMaterial()->setBloom(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            meshInstance->getMaterial()->setAmbient(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            meshInstance->getMaterial()->setDiffuse(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        else
        {
            meshInstance->getMaterial()->setBloom(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
            meshInstance->getMaterial()->setAmbient(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
            meshInstance->getMaterial()->setDiffuse(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
        }
    }
}

void    SidebarItem::setBorderEntity(Entity* borderEntity)
{
    this->_border = borderEntity;
}
