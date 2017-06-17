/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>

#include <Game/Hud/SidebarText.hpp>

REGISTER_SCRIPT(SidebarText);

void SidebarText::start() {}

void SidebarText::update(float dt) {}

void SidebarText::setParentPos(glm::vec2 parentPos, ePosition position)
{
    this->_parentPos = parentPos;

    sUiComponent* uiComponent = this->getComponent<sUiComponent>();

    if (uiComponent == nullptr)
        EXCEPT(NullptrException, "%s don't have component : %s", "SidebarText", "sUiComponent");

    if (position == ePosition::BELOW)
        uiComponent->offset = glm::vec2(this->_parentPos.x - 1, this->_parentPos.y - 4);
    else if (position == ePosition::TOP_LEFT)
        uiComponent->offset = glm::vec2(this->_parentPos.x - 1, this->_parentPos.y + 5);
}
