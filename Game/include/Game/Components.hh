#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <ECS/Component.hh>

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Graphics/Animation.hpp>
#include <Engine/Graphics/Sprite.hpp>


struct sRenderComponent: sComponent
{
    sRenderComponent() = default;
    sRenderComponent(Sprite::eType type, const std::string& texture, bool animated = false, glm::vec2 frames = {0, 0}, const std::vector<eOrientation>& orientations = {}, const glm::vec2& spriteSize = {0, 0})
    : texture(texture), type(type), animated(animated), frames(frames), orientations(orientations), spriteSize(spriteSize), _sprite(nullptr), spriteSheetOffset(0, 0) {}

    virtual sComponent* clone()
    {
        sRenderComponent* component = new sRenderComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sRenderComponent* component)
    {
        this->texture = component->texture;
        this->color = component->color;
        this->type = component->type;
        this->animated = component->animated;
        this->frames = component->frames;
        this->spriteSize = component->spriteSize;
        this->orientations = component->orientations;
        this->spriteSheetOffset = component->spriteSheetOffset;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sRenderComponent*>(component));
    }

    std::string texture;
    glm::vec3 color;
    Sprite::eType type;
    bool animated;

    // Frames numbers in sprite sheet
    glm::uvec2 frames;

    // Sprite sheet offset
    glm::vec2 spriteSheetOffset;

    // Sprite width and height
    glm::vec2 spriteSize;
    Sprite* _sprite;

    std::vector<eOrientation> orientations;
};

struct sPositionComponent: sComponent
{
    sPositionComponent() = default;
    sPositionComponent(glm::vec2 value, float z): value(value), z(z) {}

    virtual sComponent* clone()
    {
        sPositionComponent* component = new sPositionComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sPositionComponent* component)
    {
        this->value = component->value;
        this->z = component->z;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sPositionComponent*>(component));
    }

    glm::vec2 value;

    // Layer number
    float z;
};

struct sInputComponent: sComponent {
    sInputComponent() = default;
    sInputComponent(Keyboard::eKey moveLeft,
                    Keyboard::eKey moveRight,
                    Keyboard::eKey moveUp,
                    Keyboard::eKey moveDown): moveLeft(moveLeft), moveRight(moveRight), moveUp(moveUp), moveDown(moveDown) {}

    virtual sComponent* clone()
    {
        sInputComponent* component = new sInputComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sInputComponent* component)
    {
        this->moveLeft = component->moveLeft;
        this->moveRight = component->moveRight;
        this->moveUp = component->moveUp;
        this->moveDown = component->moveDown;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sInputComponent*>(component));
    }

    Keyboard::eKey          moveLeft;
    Keyboard::eKey          moveRight;
    Keyboard::eKey          moveUp;
    Keyboard::eKey          moveDown;
};

struct sDirectionComponent : sComponent
{
    sDirectionComponent() = default;
    sDirectionComponent(const glm::vec2& dir, float speed = 1.0f) : value(dir), orientation(eOrientation::N), moved(false), speed(speed) {}

    virtual sComponent* clone()
    {
        sDirectionComponent* component = new sDirectionComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sDirectionComponent* component)
    {
        this->value = component->value;
        this->orientation = component->orientation;
        this->speed = component->speed;
        this->moved = component->moved;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sDirectionComponent*>(component));
    }

    glm::vec2 value;
    eOrientation orientation;
    float speed;
    bool moved;
};

struct sRectHitboxComponent : sComponent
{
    sRectHitboxComponent() = default;
    sRectHitboxComponent(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}

    virtual sComponent* clone()
    {
        sRectHitboxComponent* component = new sRectHitboxComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sRectHitboxComponent* component)
    {
        this->min = component->min;
        this->max = component->max;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sRectHitboxComponent*>(component));
    }

    glm::vec2 min;
    glm::vec2 max;
};

struct sCircleHitboxComponent : sComponent
{
    sCircleHitboxComponent() = default;
    sCircleHitboxComponent(glm::vec2 center, float radius) : center(center), radius(radius) {}

    virtual sComponent* clone()
    {
        sCircleHitboxComponent* component = new sCircleHitboxComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sCircleHitboxComponent* component)
    {
        this->center = component->center;
        this->radius = component->radius;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sCircleHitboxComponent*>(component));
    }

    glm::vec2 center;
    float radius;
};

struct sGravityComponent : sComponent
{
    sGravityComponent() = default;
    sGravityComponent(glm::vec2 value) : value(value) {}

    virtual sComponent* clone()
    {
        sGravityComponent* component = new sGravityComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sGravityComponent* component)
    {
        this->value = component->value;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sGravityComponent*>(component));
    }

    glm::vec2 value;
};

enum class eEntityType
{
    CHARACTER = 0,
    OBJECT = 1,
    TILE_WALKABLE = 2,
    TILE_NOT_WALKABLE = 3,
    TILE_STAIRS_UP = 4,
    TILE_STAIRS_DOWN = 5
};

struct sTypeComponent: sComponent
{
    sTypeComponent() = default;
    sTypeComponent(eEntityType type): type(type) {}

    virtual sComponent* clone()
    {
        sTypeComponent* component = new sTypeComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sTypeComponent* component)
    {
        this->type = component->type;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sTypeComponent*>(component));
    }

    eEntityType type;
};

struct sAIComponent : sComponent
{
    sAIComponent() = default;

    virtual sComponent* clone()
    {
        sAIComponent* component = new sAIComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sAIComponent* component){}

    virtual void update(sComponent* component)
    {
        update(static_cast<sAIComponent*>(component));
    }
};


struct sParticleEmitterComponent : sComponent
{
    sParticleEmitterComponent() = default;

    virtual sComponent* clone()
    {
        sParticleEmitterComponent* component = new sParticleEmitterComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sParticleEmitterComponent* component)
    {
        this->rate = component->rate;
        this->spawnNb = component->spawnNb;
        this->life = component->life;
        this->lifeVariance = component->lifeVariance;
        this->angle = component->angle;
        this->angleVariance = component->angleVariance;
        this->speed = component->speed;
        this->speedVariance = component->speedVariance;

        this->colorStart = component->colorStart;
        this->colorFinish = component->colorFinish;
        this->colorStartVariance = component->colorStartVariance;
        this->colorFinishVariance = component->colorFinishVariance;

        this->sizeStart = component->sizeStart;
        this->sizeFinish = component->sizeFinish;
        this->sizeStartVariance = component->sizeStartVariance;
        this->sizeFinishVariance = component->sizeFinishVariance;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sParticleEmitterComponent*>(component));
    }

    // Spawn particles each rate second
    float rate;

    // Spawn spawnNb particles each rate
    uint32_t spawnNb;

    // Particle life time in frame
    uint32_t life;
    uint32_t lifeVariance;

    // Particles spawn angle
    float angle;
    float angleVariance;

    // Particles speed
    float speed;
    float speedVariance;

    // Particles color
    glm::vec4 colorStart;
    glm::vec4 colorFinish;
    glm::vec4 colorStartVariance;
    glm::vec4 colorFinishVariance;

    // Particles size
    float sizeStart;
    float sizeFinish;
    float sizeStartVariance;
    float sizeFinishVariance;
};

struct sNameComponent : sComponent
{
    sNameComponent() = default;
    sNameComponent(const std::string& name): value(name) {}

    virtual sComponent* clone()
    {
        sNameComponent* component = new sNameComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sNameComponent* component)
    {
        this->value = component->value;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sNameComponent*>(component));
    }

    std::string value;
};
