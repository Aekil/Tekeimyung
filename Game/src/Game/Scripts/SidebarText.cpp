/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/SidebarText.hpp>

void SidebarText::start()
{
    
}

void SidebarText::update(float dt)
{

}

void SidebarText::setParentPos(glm::vec2 parentPos)
{
    this->_parentPos = parentPos;

    sUiComponent* uiComponent = this->getComponent<sUiComponent>();

    if (uiComponent == nullptr)
        EXCEPT(NullptrException, "%s don't have component : %s", "SidebarText", "sUiComponent");
    uiComponent->offset = glm::vec2(this->_parentPos.x - 1, this->_parentPos.y - 4);
}