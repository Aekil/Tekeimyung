#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ECS/Component.hh>

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Graphics/Animation.hpp>
#include <Engine/Graphics/Sprite.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Utils/Exception.hpp>


struct sRenderComponent: sComponent
{
    sRenderComponent() = default;
    sRenderComponent(const std::string& modelFile, bool animated = false)
    : modelFile(modelFile), animated(animated) {}

    virtual sComponent* clone()
    {
        sRenderComponent* component = new sRenderComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sRenderComponent* component)
    {
        this->modelFile = component->modelFile;
        this->color = component->color;
        this->animated = component->animated;
        this->_model = component->_model;
        this->geometry = component->geometry;
        this->geometryInfo = component->geometryInfo;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sRenderComponent*>(component));
    }

    void initModel()
    {
        // Load mesh
        if (geometry == Geometry::eType::MESH)
        {
            _model = RessourceManager::getInstance()->getModel(modelFile);
            return;
        }

        // Load geometry
        if (geometry == Geometry::eType::PLANE)
        {
            _model = std::make_shared<Plane>(geometryInfo.plane);
        }
        else if (geometry == Geometry::eType::BOX)
        {
            _model = std::make_shared<Box>(geometryInfo.box);
        }
        else
            EXCEPT(InvalidParametersException, "Unknown geometry for sRenderComponent");
    }

    std::string modelFile;
    glm::vec4 color;
    bool animated;

    std::shared_ptr<Model> _model;

    // The object to render is a geometry
    Geometry::eType geometry;
    struct sGeometryInfo
    {
        Plane::sInfo    plane;
        Box::sInfo      box;
    } geometryInfo;
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

    //  X and Y coordinates
    glm::vec2 value;

    //  Z coordinates (aka layer)
    float   z;
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
    sDirectionComponent(const glm::vec2& dir, float speed = 1.0f) : value(dir), moved(false), speed(speed) {}

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
    glm::vec3 orientation;
    float speed;
    bool moved;
};

struct sBoxColliderComponent : sComponent
{
    sBoxColliderComponent() = default;

    virtual sComponent* clone()
    {
        sBoxColliderComponent* component = new sBoxColliderComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sBoxColliderComponent* component)
    {
        this->pos = component->pos;
        this->size = component->size;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sBoxColliderComponent*>(component));
    }

    // Relative position to sTransformComponent
    glm::vec3 pos;

    // Scale
    glm::vec3 size;

    // Box model
    std::shared_ptr<Box> box;
};

struct sSphereColliderComponent : sComponent
{
    sSphereColliderComponent() = default;

    virtual sComponent* clone()
    {
        sSphereColliderComponent* component = new sSphereColliderComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sSphereColliderComponent* component)
    {
        this->pos = component->pos;
        this->radius = component->radius;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sSphereColliderComponent*>(component));
    }

    glm::vec3 pos;
    float radius;
    std::shared_ptr<Sphere> sphere;
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

struct sPlayerComponent : sComponent
{
    sPlayerComponent() = default;

    virtual sComponent* clone()
    {
        sPlayerComponent* component = new sPlayerComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sPlayerComponent* component) {}

    virtual void update(sComponent* component)
    {
        update(static_cast<sPlayerComponent*>(component));
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
        this->emitterLife = component->emitterLife;
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

        this->texture = component->texture;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sParticleEmitterComponent*>(component));
    }

    // Spawn particles each rate second
    float rate;

    // Spawn spawnNb particles each rate
    uint32_t spawnNb;

    // Emitter life, in seconds
    float emitterLife;

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

    // Particles texture
    std::string texture;
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

struct sTowerAIComponent : sComponent
{
    sTowerAIComponent() = default;

    virtual sComponent* clone()
    {
        sTowerAIComponent*  component = new sTowerAIComponent();
        component->update(this);

        return (component);
    }

    virtual void    update(sTowerAIComponent* component)
    {
        this->radius = component->radius;
        this->fireRate = component->fireRate;
        this->projectileSpeed = component->projectileSpeed;
    }

    virtual void    update(sComponent* component)
    {
        update(static_cast<sTowerAIComponent*>(component));
    }

    float   radius;
    float   projectileSpeed;

    float   fireRate;
    float   lastShotTime;

    uint32_t    targetId;
};

struct  sProjectileComponent : sComponent
{
    sProjectileComponent() = default;

    virtual sComponent* clone()
    {
        sProjectileComponent*  component = new sProjectileComponent();
        component->update(this);

        return (component);
    }

    virtual void    update(sProjectileComponent* component)
    {
        this->shooterId = component->shooterId;
        this->guided = component->guided;
        this->rangeMax = component->rangeMax;
        this->targetId = component->targetId;
    }

    virtual void    update(sComponent* component)
    {
        update(static_cast<sProjectileComponent*>(component));
    }

    uint32_t    shooterId;

    bool        guided;
    float       rangeMax;
    uint32_t    targetId;
};

struct sWaveComponent : sComponent
{
    sWaveComponent() = default;

    virtual sComponent* clone()
    {
        sWaveComponent* component = new sWaveComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sWaveComponent* component)
    {
        this->spawnPos = component->spawnPos;
        this->secBeforeSpawn = component->secBeforeSpawn;
        this->nSpawn = component->nSpawn;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sWaveComponent*>(component));
    }

    glm::vec3   spawnPos;
    float       secBeforeSpawn;
    int         nSpawn;
};


struct sTransformComponent : sComponent
{
    sTransformComponent(): scale({1.0f, 1.0f, 1.0f}), transform(1.0f), needUpdate(true) {}

    virtual sComponent* clone()
    {
        sTransformComponent* component = new sTransformComponent();
        component->update(this);

        return (component);
    }

    virtual void update(sTransformComponent* component)
    {
        this->pos = component->pos;
        this->scale = component->scale;
        this->rotation = component->rotation;
        this->transform = component->transform;
    }

    virtual void update(sComponent* component)
    {
        update(static_cast<sTransformComponent*>(component));
    }

    const glm::mat4& getTransform()
    {
        if (needUpdate)
        {
            glm::mat4 orientation;
            orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            orientation = glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

            needUpdate = false;
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(pos.x, pos.y, pos.z)) * orientation;
            transformMatrix = glm::scale(transformMatrix, scale);
            transform = transformMatrix;
        }

        return (transform);
    }

    glm::vec3   pos;
    glm::vec3   scale;
    glm::vec3   rotation;
    glm::mat4   transform;
    bool        needUpdate;
};
