/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define MENU_SYSTEM_NAME "Menu system"

START_SYSTEM(MenuSystem)
public:
    MenuSystem();
    virtual ~MenuSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

private:
    void                                removeSelected(Entity* entity);
    void                                setupSelectedIcon();

    void                                handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos);
    void                                handleButtonsKeys(EntityManager& em);
    void                                handleAlignment(EntityManager& em, Entity* entity, uint32_t entityIdx);

    void                                setSelected(EntityManager &em, int buttonIdx, bool hovered = false);
    void                                setSelected(Entity* entity, bool hovered = false);
    void                                removeSelected(EntityManager &em, int buttonIdx);

private:
    ADD_MONITORING_VAR

    int                                 _currentSelected;
    bool                                _buttonHovered;

    Entity*                             _iconSelected = nullptr;
    sRenderComponent*                   _iconRender = nullptr;
END_SYSTEM(MenuSystem)
