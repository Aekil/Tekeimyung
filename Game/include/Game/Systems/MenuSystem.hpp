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
    virtual bool                        onEntityNewComponent(Entity* entity, sComponent* component);

private:
    void                                handleMouseHover(EntityManager &em);

    void                                setSelected(EntityManager &em, int buttonIdx);
    void                                setSelected(Entity* entity);
    void                                removeSelected(EntityManager &em, int buttonIdx);

private:
    ADD_MONITORING_VAR

    int                                 _currentSelected;
    bool                                _buttonHovered;
END_SYSTEM(MenuSystem)
