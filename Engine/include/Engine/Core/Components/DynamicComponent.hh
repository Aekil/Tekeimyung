#pragma once

#include <ECS/Component.hh>

START_COMPONENT(sDynamicComponent)
virtual sComponent* clone()
{
    sDynamicComponent* component = new sDynamicComponent();
    component->update(this);

    return (component);
}

virtual void update(sDynamicComponent* component)
{
}

virtual void update(sComponent* component)
{
    update(static_cast<sDynamicComponent*>(component));
}
END_COMPONENT(sDynamicComponent)
