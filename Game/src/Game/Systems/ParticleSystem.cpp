#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>

#include <Game/Map.hpp>

#include <Game/Systems/ParticleSystem.hpp>

ParticleSystem::ParticleSystem()
{
    addDependency<sParticleEmitterComponent>();
    addDependency<sPositionComponent>();
}

ParticleSystem::~ParticleSystem() {}

void    ParticleSystem::initEmitter(Entity* entity)
{
    sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
    sRenderComponent *sprite = entity->getComponent<sRenderComponent>();
    sEmitter* emitter = new sEmitter();

    emitter->particles.resize(MAX_PARTICLES);
    emitter->particlesNb = 0;
    _emitters[entity->id] = emitter;
}


void    ParticleSystem::updateEmitter(EntityManager &em, Entity* entity, float elapsedTime)
{
    sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    sEmitter* emitter = _emitters[entity->id];

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
            removeEmitter(entity->id);
            em.destroyEntityRegister(entity);
        }
    }
    // Create new particles each rate
    else if (emitter->timer.getElapsedTime() >= emitterComp->rate)
    {
        for (uint32_t i = 0; i < emitterComp->spawnNb; i++)
        {
            sParticle particle;
            float angle = Helper::randFloat(0, emitterComp->angleVariance);
            float angleRadian = glm::radians(std::fmod(angle - emitterComp->angle, 360.0f));

            particle.pos = transform->pos;
            float theta = Helper::randFloat(0.0f, 1.0f) * (2.0f * glm::pi<float>());
            float phi = (glm::pi<float>() / 2.0f) - (Helper::randFloat(0.0f, 1.0f) * angleRadian);
            particle.velocity.x = glm::cos(theta) * glm::cos(phi);
            particle.velocity.z = glm::sin(theta) * glm::cos(phi);
            particle.velocity.y =  glm::sin(phi);
            particle.speed = emitterComp->speed + Helper::randFloat(0, emitterComp->speedVariance);
            particle.life = emitterComp->life + Helper::randInt(0, emitterComp->lifeVariance);

            particle.color = emitterComp->colorStart;
            particle.colorStep = (emitterComp->colorFinish - emitterComp->colorStart) / glm::vec4((float)emitterComp->life);

            particle.size = emitterComp->sizeStart;
            particle.sizeStep = (emitterComp->sizeFinish - emitterComp->sizeStart) / emitterComp->life;

            // Can add a particle in particles list
            if (emitter->particlesNb != emitter->particles.size())
            {
                emitter->particles[emitter->particlesNb] = particle;
                emitter->particlesNb++;
            }
        }
        emitter->timer.reset();
    }
}

void    ParticleSystem::removeEmitter(uint32_t id)
{
    sEmitter* emitter = _emitters[id];

    // Delete emitter pointer
    delete emitter;
    // Remove emitter from map
    _emitters.erase(_emitters.find(id));
}

void    ParticleSystem::update(EntityManager &em, float elapsedTime)
{
    uint32_t activeEmitters = 0;

    // Iterate over particle emitters
    forEachEntity(em, [&](Entity *entity) {


        // Emitter not initialized
        if (_emitters.find(entity->id) == _emitters.end())
            initEmitter(entity);

        updateEmitter(em, entity, elapsedTime);
        activeEmitters++;
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
                uint32_t emitterId = it->first;
                ++it;
                removeEmitter(emitterId);
            }
            else
                ++it;
        }
    }
}

std::unordered_map<uint32_t, sEmitter*>* ParticleSystem::getEmitters()
{
    return (&_emitters);
}
