#pragma once

#include <unordered_map>
#include <vector>

#include <ECS/System.hpp>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>

#include <Game/Components.hh>

#define MAX_PARTICLES   5000

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
    float size;
    // Increment size by sizeStep each frame
    float sizeStep;
};

struct sEmitter
{
    std::vector<sParticle>  particles;
    unsigned int            particlesNb;
    float                   life;
    Timer                   timer;
    std::shared_ptr<Plane>  model;
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
    void            updateEmitter(EntityManager &em, Entity* entity, float elapsedTime);
    void            removeEmitter(uint32_t id);

private:
    std::unordered_map<uint32_t, sEmitter*>     _emitters;
};
