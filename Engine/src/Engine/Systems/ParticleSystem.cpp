/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>

#include <Engine/Systems/ParticleSystem.hpp>

// We can't initialize the buffer pool because opengl is not initialized
std::unique_ptr<BufferPool> ParticleSystem::_bufferPool = nullptr;

ParticleSystem::ParticleSystem(bool editorMode): _editorMode(editorMode)
{
    addDependency<sParticleEmitterComponent>();
    addDependency<sRenderComponent>();

    if (!_bufferPool)
    {
        _bufferPool = std::make_unique<BufferPool>(1,
                                    (uint32_t)(sizeof(glm::mat4) + sizeof(glm::vec4)) * MAX_PARTICLES,
                                    GL_SHADER_STORAGE_BUFFER);
    }
}

ParticleSystem::~ParticleSystem() {}

void    ParticleSystem::initEmitter(Entity* entity)
{
    sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
    sEmitter* emitter = new sEmitter();

    emitter->particles.resize(MAX_PARTICLES);
    emitter->particlesNb = 0;
    emitter->elapsedTime = 0;
    emitter->buffer = _bufferPool->allocate();

    _emitters[entity->handle] = emitter;
}


void    ParticleSystem::updateEmitter(EntityManager &em, Entity* entity, float elapsedTime)
{
    sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    sRenderComponent *render = entity->getComponent<sRenderComponent>();
    sEmitter* emitter = _emitters[entity->handle];

    emitter->elapsedTime += elapsedTime;
    // Update particles
    for (unsigned int i = 0; i < emitter->particlesNb; i++)
    {
        glm::vec3 velocity;

        velocity = emitter->particles[i].velocity * elapsedTime * emitter->particles[i].speed;

        emitter->particles[i].pos += velocity;
        emitter->particles[i].life--;
        emitter->particles[i].color += emitter->particles[i].colorStep;
        emitter->particles[i].size += emitter->particles[i].sizeStep;

        // Remove particle
        if (emitter->particles[i].life <= 0)
        {
            unsigned int j = 0;
            std::remove_if(emitter->particles.begin(), emitter->particles.end(), [&](sParticle& particle){
                j++;
                return (j - 1 == i);
            });
            --i;
            --emitter->particlesNb;
        }
    }

    // Update emitter life time
    if (emitterComp->emitterLife)
        emitter->life += elapsedTime;

    // The emitter is not alive
    if (emitterComp->emitterLife &&
        emitter->life >= emitterComp->emitterLife)
    {
        // Wait for particles deletion and remove emitter
        if (emitter->particlesNb == 0)
        {
            // We don't want to destroy the emitter in editor mode
            // We just reset the life of the emitter
            if (_editorMode)  {
                emitter->life = 0;
            }
            else {
                removeEmitter(entity->handle);
                em.destroyEntityRegister(entity->handle);
            }
        }
    }
    // Create new particles each rate
    else if (emitter->elapsedTime >= emitterComp->rate &&
        render->_display &&
        emitter->particlesNb < MAX_PARTICLES)
    {
        if (emitter->particlesNb >= emitter->particles.size() || // Vector is full
            emitter->particlesNb >= emitterComp->maxParticles) // Particles limit
        {
            return;
        }

        glm::mat4 emitterOrientation;
        emitterOrientation = glm::rotate(emitterOrientation, glm::radians(transform->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        emitterOrientation = glm::rotate(emitterOrientation, glm::radians(transform->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        emitterOrientation = glm::rotate(emitterOrientation, glm::radians(transform->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

        sParticle particle;
        float angle = Helper::randFloat(0, emitterComp->angleVariance);
        float angleRadian = glm::radians(std::fmod(angle - emitterComp->angle, 360.0f));

        particle.pos = transform->getPos();
        float theta = Helper::randFloat(0.0f, 1.0f) * (2.0f * glm::pi<float>());
        float phi = (glm::pi<float>() / 2.0f) - (Helper::randFloat(0.0f, 1.0f) * angleRadian);
        particle.velocity.x = glm::cos(theta) * glm::cos(phi);
        particle.velocity.z = glm::sin(theta) * glm::cos(phi);
        particle.velocity.y =  glm::sin(phi);

        particle.velocity = glm::vec3(emitterOrientation * glm::vec4(particle.velocity, 0.0f));

        particle.speed = emitterComp->speed + Helper::randFloat(0, emitterComp->speedVariance);
        particle.life = emitterComp->life + Helper::randInt(0, emitterComp->lifeVariance);

        particle.color = emitterComp->colorStart;
        particle.colorStep = (emitterComp->colorFinish - emitterComp->colorStart) / glm::vec4((float)emitterComp->life);

        particle.size = emitterComp->sizeStart * transform->getScale();
        particle.sizeStep = ((emitterComp->sizeFinish - emitterComp->sizeStart) / emitterComp->life) * transform->getScale();

        // Add particle
        emitter->particles[emitter->particlesNb] = particle;
        emitter->particlesNb++;

        emitter->elapsedTime = 0;
    }
}

void    ParticleSystem::removeEmitter(const Entity::sHandle& handle)
{
    sEmitter* emitter = _emitters[handle];

    _bufferPool->free(emitter->buffer);

    // Delete emitter pointer
    delete emitter;
    // Remove emitter from map
    _emitters.erase(_emitters.find(handle));
}

void    ParticleSystem::update(EntityManager &em, float elapsedTime)
{
    uint32_t activeEmitters = 0;

    // Iterate over particle emitters
    forEachEntity(em, [&](Entity *entity) {


        // Emitter not initialized
        if (_emitters.find(entity->handle) == _emitters.end())
            initEmitter(entity);

        updateEmitter(em, entity, elapsedTime);
        ++activeEmitters;
    });

    // 1 or more emitters have been deleted
    // Update the emitter map
    if (activeEmitters != _emitters.size())
    {
        auto it = _emitters.cbegin();
        while (it != _emitters.cend())
        {
            Entity* entity = em.getEntity(it->first);
            // The emitter has been deleted or the component has been removed, remove it from the map
            if (!entity ||
                !entity->getComponent<sParticleEmitterComponent>())
            {
                const Entity::sHandle& emitterHandler = it->first;
                ++it;
                removeEmitter(emitterHandler);
            }
            else
                ++it;
        }
    }
}

std::unordered_map<Entity::sHandle, sEmitter*>* ParticleSystem::getEmitters()
{
    return (&_emitters);
}
