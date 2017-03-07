/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <ECS/crc32.hh>

struct sComponent
{
    sComponent() {}
    sComponent(uint32_t id): id(id) {}
    virtual ~sComponent() {}

    virtual sComponent* clone() = 0;
    virtual void        update(sComponent* component) = 0;
    uint32_t id;
};


#define START_COMPONENT(name) \
    struct name : sComponent { \
        name(): sComponent(name::identifier) {} \
        static constexpr unsigned int identifier = #name##_crc32;

// TODO: Add optional parameter to START_COMPONENT
#define START_COMPONENT_INHERIT(name, baseClass) \
    struct name : sComponent, public baseClass { \
        name(): sComponent(name::identifier) {} \
        static constexpr unsigned int identifier = #name##_crc32;

#define END_COMPONENT(name) \
    };
