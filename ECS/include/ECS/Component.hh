#pragma once

#include <cstdint>
#include <ECS/crc32.hh>

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
