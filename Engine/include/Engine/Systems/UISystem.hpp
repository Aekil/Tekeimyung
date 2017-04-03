/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Components.hh>

START_SYSTEM(UISystem)
public:
    UISystem();
    virtual ~UISystem();

    virtual void                        update(EntityManager &em, float elapsedTime);

    void                                onWindowResize(EntityManager &em);

private:
    void                                handleAlignment(EntityManager& em, Entity* entity, bool forceUpdate = false);
    void                                alignText(sTextComponent* textComp, const glm::vec3& uiSize);
END_SYSTEM(UISystem)
