#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

#include "Window/GameWindow.hpp"
#include "Utils/Helper.hpp"

#include "Systems/ParticleSystem.hpp"

ParticleSystem::ParticleSystem()
{
    addDependency<sParticleEmitterComponent>();
    addDependency<sPositionComponent>();
    addDependency<sRenderComponent>();
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


void    ParticleSystem::updateEmitter(Entity* entity, float elapsedTime)
{
    sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
    sPositionComponent *pos = entity->getComponent<sPositionComponent>();
    //sRenderComponent *sprite = entity->getComponent<sRenderComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
    sEmitter* emitter = _emitters[entity->id];

/*    if (!sprite->_sprite)
        return;*/

    bool moved = direction && direction->moved;
    eOrientation orientation = direction ? direction->orientation : eOrientation::N;
    //sprite->_sprite->update(pos->value, pos->z, moved, orientation, sprite->color);

    // Update particles
    for (unsigned int i = 0; i < emitter->particlesNb; i++)
    {
        glm::vec3 velocity;

        velocity.x = emitter->particles[i].velocity.x * elapsedTime * emitter->particles[i].speed;
        velocity.y = emitter->particles[i].velocity.y * elapsedTime * emitter->particles[i].speed;

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

    // Create new particles each second
    if (emitter->timer.getElapsedTime() >= emitterComp->rate)
    {
        for (uint32_t i = 0; i < emitterComp->spawnNb; i++)
        {
            sParticle particle;
            float angle = Helper::randFloat(0, emitterComp->angleVariance);
            float angleRadian = glm::radians(std::fmod(angle - emitterComp->angle, 360.0f));

            //particle.pos = sprite->_sprite->getPos();
            particle.velocity.x = glm::cos(angleRadian);
            particle.velocity.y = glm::sin(angleRadian);
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

void    ParticleSystem::update(EntityManager &em, float elapsedTime)
{
    // Iterate over particle emitters
    forEachEntity(em, [&](Entity *entity) {


        // Emitter not initialized
        if (_emitters.find(entity->id) == _emitters.end())
            initEmitter(entity);

        updateEmitter(entity, elapsedTime);
    });
}

std::unordered_map<uint32_t, sEmitter*>* ParticleSystem::getEmitters()
{
    return (&_emitters);
}
