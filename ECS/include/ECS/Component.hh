#pragma once

#include <typeinfo>
#include <cstdint>
#include <ECS/crc32.hh>

/*
struct BaseComponent {
    BaseComponent(unsigned int identifier) : id{identifier} {}
    unsigned int id;
};

template <typename ComponentType>
struct Component : BaseComponent {
    Component() : BaseComponent{ComponentType::identifier} {}
};

#define COMPONENT(name) \
    struct name : Component<name> { \
        static constexpr unsigned int identifier = #name##_crc32;

#define END_COMPONENT(name) \
    };

COMPONENT(ComponentHealth)
    int hp;
END_COMPONENT(ComponentHealth)

COMPONENT(ComponentSpeed)
    int hp;
END_COMPONENT(ComponentSpeed)

/////////////////////////////////////
// Check type of component
/////////////////////////////////////

template<typename ComponentType>
ComponentType* getComponent(BaseComponent *component) {
    if (component && component->id == ComponentType::identifier) {
        return static_cast<ComponentType*>(component);
    }

    return nullptr;
}
*/

struct sComponent
{
    sComponent() {}
    sComponent(uint32_t id): id(id) {}

    virtual sComponent* clone() = 0;
    virtual void        update(sComponent* component) = 0;
    uint32_t id;
};


#define START_COMPONENT(name) \
    struct name : sComponent { \
        name(): sComponent(name::identifier) {} \
        static constexpr unsigned int identifier = #name##_crc32;

#define END_COMPONENT(name) \
    };
