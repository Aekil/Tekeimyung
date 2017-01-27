/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Components.hh>

#define MENU_SYSTEM_NAME "Menu system"

START_SYSTEM(MenuSystem)
public:
    MenuSystem();
    virtual ~MenuSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

    void                                onWindowResize(EntityManager &em);

    virtual bool                        onEntityNewComponent(Entity* entity, sComponent* component);
    virtual bool                        onEntityRemovedComponent(Entity* entity, sComponent* component);
    virtual bool                        onEntityDeleted(Entity* entity);

private:
    void                                removeSelected(Entity* entity);
    void                                setupSelectedIcon();

    void                                handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos);
    void                                handleAlignment(EntityManager& em, Entity* entity, uint32_t entityIdx, bool forceUpdate = false);

    void                                handleButtonsKeys(EntityManager& em);

    void                                setSelected(EntityManager &em, int buttonIdx, bool hovered = false);
    void                                setSelected(Entity* entity, bool hovered = false);
    void                                removeSelected(EntityManager &em, int buttonIdx);


private:
    ADD_MONITORING_VAR

    int                                 _currentSelected;
    bool                                _buttonHovered;
    bool                                _recreateIcon = false;

    Entity*                             _iconSelected = nullptr;
    sRenderComponent*                   _iconRender = nullptr;

    std::vector<uint32_t>               _buttons;
END_SYSTEM(MenuSystem)
