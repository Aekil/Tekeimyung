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
    void                                setSelected(Entity* entity);
    void                                removeSelected(Entity* entity);

private:
    ADD_MONITORING_VAR

    int                                 _currentSelected;
END_SYSTEM(MenuSystem)
