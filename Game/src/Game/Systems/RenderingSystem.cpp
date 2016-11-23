#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>


RenderingSystem::RenderingSystem(Map* map, std::unordered_map<uint32_t, sEmitter*>* particleEmitters): _map(map), _particleEmitters(particleEmitters)
{
    addDependency<sPositionComponent>();
    addDependency<sRenderComponent>();

    _camera.translate(glm::vec3(350.0f, 250.0f, 300.0f));
    _camera.setDir(glm::vec3(-30.0f));
    _camera.getUbo().bind(_shaderProgram, "camera");
    Camera::setInstance(&_camera);
}

RenderingSystem::~RenderingSystem() {}

bool    RenderingSystem::init()
{
    try
    {
        _shaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
        _shaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag");
        _shaderProgram.link();
        _shaderProgram.use();

        // Set texture location unit
        // Must be the same unit as material textures. See Material::loadFromAssimp
        glUniform1i(_shaderProgram.getUniformLocation("AmbientTexture"), 0);
        glUniform1i(_shaderProgram.getUniformLocation("DiffuseTexture"), 1);
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    // Enable blend for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Activate back culling
    //glEnable(GL_CULL_FACE);
    return (true);
}

void    RenderingSystem::renderEntity(Entity* entity)
{
    auto&& model = getModel(entity);

    // Draw model
    model->draw(_shaderProgram);
}

/*
void    RenderingSystem::renderEntities(EntityManager& em, std::list<uint32_t>::const_iterator& it, uint16_t layer, uint32_t x, uint32_t y)
{
    Entity* entity;
    sPositionComponent *position;

    if (it == (*_map)[layer].getEntities().cend())
        return;

    entity = em.getEntity(*it);
    ASSERT(entity != nullptr, "The entity should exists");

    position = entity->getComponent<sPositionComponent>();
    while (std::floor(position->value.x) == x && std::floor(position->value.y) == y)
    {
        renderEntity(entity);
        it++;

        if (it == (*_map)[layer].getEntities().cend())
            break;

        entity = em.getEntity(*it);
        ASSERT(entity != nullptr, "The entity should exists");

        position = entity->getComponent<sPositionComponent>();
    }
}*/


void    RenderingSystem::renderCollider(Entity* entity)
{
    auto&& model = getModel(entity);

    sBoxColliderComponent* boxCollider = entity->getComponent<sBoxColliderComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    if (boxCollider)
    {
        if (!boxCollider->box)
        {
            Box::sInfo boxInfo;
            boxInfo.width = boxCollider->size.x;
            boxInfo.height = boxCollider->size.y;
            boxInfo.length = boxCollider->size.z;
            boxCollider->box = std::make_shared<Box>(boxInfo);
        }

        glm::mat4 boxTransform = transform->transform;
        boxTransform = glm::translate(boxTransform, boxCollider->pos);

        boxCollider->box->update(glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), boxTransform);
        boxCollider->box->draw(_shaderProgram);
    }
}

void    RenderingSystem::renderParticles(EntityManager& em)
{
    // Activate additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (auto &&it: *_particleEmitters)
    {
        auto &&emitter = it.second;
        Entity* entity = em.getEntity(it.first);
        if (entity == nullptr)
            continue;

        sRenderComponent *render = entity->getComponent<sRenderComponent>();
        auto&& model = getModel(entity);

        // Only freeze camera rotation for plans
        if (render->geometry == Geometry::eType::PLANE)
            _camera.freezeRotations(true);

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(particle.pos.x, particle.pos.y, particle.pos.z));
            transformMatrix = glm::scale(transformMatrix, glm::vec3(particle.size, particle.size, particle.size));
            model->update(particle.color, transformMatrix);

            // Draw sprite
            model->draw(_shaderProgram);
        }

        _camera.freezeRotations(false);
    }

    // Activate transparency blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    // Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Unfree camera rotation to display normal models
   _camera.freezeRotations(false);

    _camera.update(_shaderProgram, elapsedTime);

    // Iterate over particle emitters
    forEachEntity(em, [&](Entity *entity) {
        // Don't display particle systems
        if (!entity->getComponent<sParticleEmitterComponent>())
        {


            if (!isTransparentEntity(entity))
                renderEntity(entity);
            else
                _transparentEntities[entity->id] = entity;
        }

        if (entity->getComponent<sBoxColliderComponent>())
            _collierEntities[entity->id] = entity;
    });


    // Enable blend to blend transparent ojects and particles
    glEnable(GL_BLEND);
    // Disable write to the depth buffer so that the depth of transparent objects is not written
    // because we don't want a transparent object to hide an other transparent object
    glDepthMask(GL_FALSE);

    // Display transparent entities
    {
        auto it = _transparentEntities.begin();
        while (it != _transparentEntities.end())
        {
            // Get fresh entity pointer to handle deletion
            Entity* entity = em.getEntity(it->first);
            if (!entity)
            {
                uint32_t id = it->first;
                ++it;
                _transparentEntities.erase(id);
            }
            else
            {
                renderEntity(entity);
                ++it;
            }
        }
    }

    // Display transparent colliders
    {
        auto it = _collierEntities.begin();
        while (it != _collierEntities.end())
        {
            // Get fresh entity pointer to handle deletion
            Entity* entity = em.getEntity(it->first);
            if (!entity)
            {
                uint32_t id = it->first;
                ++it;
                _collierEntities.erase(id);
            }
            else
            {
                renderCollider(entity);
                ++it;
            }
        }
    }

    renderParticles(em);

    // Enable depth buffer for opaque objects
    glDepthMask(GL_TRUE);
    // Disable blending for opaque objects
    glDisable(GL_BLEND);


    // Display imgui windows
    ImGui::Render();

    // Display screen
    GameWindow::getInstance()->display();
}

bool    RenderingSystem::isTransparentEntity(Entity* entity) const
{
    sRenderComponent *model = entity->getComponent<sRenderComponent>();

    return (model->geometry == Geometry::eType::PLANE);
}

std::shared_ptr<Model>  RenderingSystem::getModel(Entity* entity)
{
    int id = entity->id;
    sRenderComponent *model = entity->getComponent<sRenderComponent>();
    sPositionComponent *position = entity->getComponent<sPositionComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();

    // The entity does not exist in the render system
    if (!model->_model)
    {
        model->initModel();
    }

    if (transform->needUpdate)
    {
        model->_model->update(model->color, transform->getTransform());
    }
    else
        model->_model->update(model->color, transform->transform);

    return (model->_model);
}

const ShaderProgram&  RenderingSystem::getShaderProgram() const
{
    return _shaderProgram;
}
