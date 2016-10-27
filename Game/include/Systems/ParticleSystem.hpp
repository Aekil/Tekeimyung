#pragma once

# define MAX_PARTICLES 5000
#include <unordered_map>
#include <vector>
#include "Core/Components.hh"
#include "System.hpp"
#include "Utils/Timer.hpp"

struct sParticle
{
    glm::vec3 pos;
    glm::vec3 velocity;
    float speed;

    // How long a particle live, in frames
    float life;

    // Particle color
    glm::vec4 color;
    // Increment color by colorStep each frame
    glm::vec4 colorStep;

    // Particle scale
    float size;
    // Increment size by sizeStep each frame
    float sizeStep;
};

struct sEmitter
{
    std::vector<sParticle>  particles;
    unsigned int            particlesNb;
    Timer                   timer;
};

class ParticleSystem: public System
{
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    virtual void    update(EntityManager &em, float elapsedTime);
    std::unordered_map<uint32_t, sEmitter*>* getEmitters();

private:
    void            initEmitter(Entity* entity);
    void            updateEmitter(Entity* entity, float elapsedTime);

private:
    std::unordered_map<uint32_t, sEmitter*>     _emitters;
};
