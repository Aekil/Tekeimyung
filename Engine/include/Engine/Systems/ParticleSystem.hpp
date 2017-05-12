/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <vector>

#include <ECS/Entity.hpp>
#include <ECS/System.hpp>

#include <Engine/Graphics/BufferPool.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>


#define MAX_PARTICLES   2000

struct sParticle
{
    glm::vec3 pos;
    glm::vec3 velocity;
    float speed;

    // How long a particle live, in frames
    int life;

    // Particle color
    glm::vec4 color;
    // Increment color by colorStep each frame
    glm::vec4 colorStep;

    // Particle scale
    glm::vec3 size;
    // Increment size by sizeStep each frame
    glm::vec3 sizeStep;
};

struct sEmitter
{
    std::vector<sParticle>  particles;
    unsigned int            particlesNb;
    float                   life;
    float                   elapsedTime;

    // Instance ubo
    BufferPool::SubBuffer*  buffer;
};

START_SYSTEM(ParticleSystem)
public:
    ParticleSystem(bool editorMode = false);
    virtual ~ParticleSystem();

    virtual void    update(EntityManager &em, float elapsedTime);
    std::unordered_map<Entity::sHandle, sEmitter*>* getEmitters();

private:
    void            initEmitter(Entity* entity);
    void            updateEmitter(EntityManager &em, Entity* entity, float elapsedTime);
    void            removeEmitter(const Entity::sHandle& handle);

private:
    std::unordered_map<Entity::sHandle, sEmitter*>     _emitters;
    bool                                        _editorMode;
    static std::unique_ptr<BufferPool>          _bufferPool;
END_SYSTEM(ParticleSystem)
