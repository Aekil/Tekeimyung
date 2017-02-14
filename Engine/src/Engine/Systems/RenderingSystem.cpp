/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Graphics/Renderer.hpp>

#include <Engine/LevelEntitiesDebugWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>


RenderingSystem::RenderingSystem(Camera* camera, std::unordered_map<uint32_t, sEmitter*>* particleEmitters):
                                _camera(camera), _particleEmitters(particleEmitters)
{
    addDependency<sRenderComponent>();

    if (!_camera)
        _camera = &_defaultCamera;
}

RenderingSystem::~RenderingSystem() {}

bool    RenderingSystem::init()
{
    return (true);
}

void    RenderingSystem::renderEntity(sRenderComponent *render, Entity* entity, float elapsedTime)
{
    if (!render || !render->_display)
        return;

    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    auto&& model = getModel(render);

    if (!model)
        return;

    // Update animation
    if (render->_animator.isPlaying())
    {
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        render->_animator.update(elapsedTime);
        transform->needUpdate = true;
    }

    // Draw model
    if (entity->hasComponent<sUiComponent>())
        Renderer::getInstance()->renderUI(model, render->color, transform->getTransform());
    else
        Renderer::getInstance()->render(_camera, model, render->color, transform->getTransform());
}

void    RenderingSystem::renderCollider(Entity* entity)
{
    if (!entity)
        return;

    sBoxColliderComponent* boxCollider = entity->getComponent<sBoxColliderComponent>();
    sSphereColliderComponent* sphereCollider = entity->getComponent<sSphereColliderComponent>();
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    if (boxCollider && boxCollider->display)
    {
        if (!boxCollider->box)
        {
            boxCollider->box = static_cast<Box*>(GeometryFactory::getGeometry(Geometry::eType::BOX));
        }

        glm::mat4 boxTransform = transform->getTransform();
        boxTransform = glm::translate(boxTransform, boxCollider->pos);
        boxTransform = glm::scale(boxTransform, boxCollider->size);

        Renderer::getInstance()->render(_camera, boxCollider->box, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), boxTransform);
    }
    if (sphereCollider && sphereCollider->display)
    {
        if (!sphereCollider->sphere)
        {
            Sphere::sInfo sphereInfo;
            sphereInfo.radius = SIZE_UNIT;
            sphereCollider->sphere = static_cast<Sphere*>(GeometryFactory::getGeometry(Geometry::eType::SPHERE));
        }

        glm::mat4 sphereTransform = transform->getTransform();
        sphereTransform = glm::translate(sphereTransform, sphereCollider->pos);
        sphereTransform = glm::scale(sphereTransform, glm::vec3(sphereCollider->radius));

        Renderer::getInstance()->render(_camera, sphereCollider->sphere, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), sphereTransform);
    }
}

void    RenderingSystem::renderColliders(EntityManager& em)
{
    static bool displayAllColliders = false;

    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    if (keyboard.getStateMap()[Keyboard::eKey::C] == Keyboard::eKeyState::KEY_PRESSED)
        displayAllColliders = !displayAllColliders;

    if (displayAllColliders)
    {
        for (uint32_t& entityId: _collidableEntities)
        {
            Entity* entity = em.getEntity(entityId);
            if (!entity)
                continue;

            renderCollider(entity);
        }
    }

    renderCollider(em.getEntity(LevelEntitiesDebugWindow::getSelectedEntityId()));
}

void    RenderingSystem::renderParticles(EntityManager& em, float elapsedTime)
{
    if (!_particleEmitters)
        return;

    // Activate additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (auto &&it: *_particleEmitters)
    {
        auto &&emitter = it.second;
        Entity* entity = em.getEntity(it.first);
        if (entity == nullptr)
            continue;

        bool isUi = entity->hasComponent<sUiComponent>();
        sRenderComponent *render = entity->getComponent<sRenderComponent>();
        auto&& model = getModel(render);

        if (!model)
            continue;

        // Update animation
        if (render->_animator.isPlaying())
        {
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();

            render->_animator.update(elapsedTime);
            transform->needUpdate = true;
        }

        // Only freeze camera rotation for planes
        if (render->type == Geometry::eType::PLANE)
            _camera->freezeRotations(true);

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(particle.pos.x, particle.pos.y, particle.pos.z));
            transformMatrix = glm::scale(transformMatrix, particle.size);

            // Draw sprite
            if (isUi)
                Renderer::getInstance()->renderUI(model, particle.color, transformMatrix);
            else
                Renderer::getInstance()->render(_camera, model, particle.color, transformMatrix);
        }

        _camera->freezeRotations(false);
    }

    // Activate transparency blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    if (_camera->getProjType() == Camera::eProj::ORTHOGRAPHIC_2D)
        glDisable(GL_DEPTH_TEST);

    // Unfree camera rotation to display normal models
   _camera->freezeRotations(false);

    _camera->update(Renderer::getInstance()->getShaderProgram(), elapsedTime);

    forEachEntity(em, [&](Entity *entity) {
        // Don't display particle systems
        if (!entity->getComponent<sParticleEmitterComponent>())
        {
            sRenderComponent *render = entity->getComponent<sRenderComponent>();

            if (!isTransparent(render))
                renderEntity(render, entity, elapsedTime);
            else
                _transparentEntities[entity->id] = entity;
        }
    });

    // Enable blend to blend transparent ojects and particles
    glEnable(GL_BLEND);
    // Disable write to the depth buffer so that the depth of transparent objects is not written
    // because we don't want a transparent object to hide an other transparent object
    glDepthMask(GL_FALSE);


    renderColliders(em);

    // Display transparent entities
    {
        auto it = _transparentEntities.begin();
        while (it != _transparentEntities.end())
        {
            // Get fresh entity pointer to handle deletion
            Entity* entity = em.getEntity(it->first);
            if (!entity || entity->hasComponent<sParticleEmitterComponent>())
            {
                uint32_t id = it->first;
                ++it;
                _transparentEntities.erase(id);
            }
            else
            {
                sRenderComponent *render = entity->getComponent<sRenderComponent>();
                renderEntity(render, entity, elapsedTime);
                ++it;
            }
        }
    }

    renderParticles(em, elapsedTime);

    // Enable depth buffer for opaque objects
    glDepthMask(GL_TRUE);
    // Disable blending for opaque objects
    glDisable(GL_BLEND);

    if (_camera->getProjType() == Camera::eProj::ORTHOGRAPHIC_2D)
        glEnable(GL_DEPTH_TEST);
}

bool    RenderingSystem::isTransparent(sRenderComponent *render) const
{
    return (render->type == Geometry::eType::PLANE);
}

Model*  RenderingSystem::getModel(sRenderComponent *render)
{
    if (!render)
        return (nullptr);

    return (render->getModel());
}

bool    RenderingSystem::onEntityRemovedComponent(Entity* entity, sComponent* component)
{
    System::onEntityRemovedComponent(entity, component);

    // The component is a collider
    if (component->id == sBoxColliderComponent::identifier ||
        component->id == sSphereColliderComponent::identifier)
    {
        // The entity is in the collidable entities vector
        auto foundEntity = std::find(_collidableEntities.cbegin(), _collidableEntities.cend(), entity->id);
        if (foundEntity != _collidableEntities.cend())
        {
            _collidableEntities.erase(foundEntity);
            return (true);
        }
    }
    return (false);
}

bool    RenderingSystem::onEntityCreated(Entity* entity)
{
    // The entity match RenderingSystem dependencies
    // and the new component is a collider
    if (entityMatchDependencies(entity) &&
        (entity->hasComponent<sBoxColliderComponent>() ||
        entity->hasComponent<sSphereColliderComponent>()))
    {
        // The entity is not already in the collidable entities vector
        if (std::find(_collidableEntities.cbegin(), _collidableEntities.cend(), entity->id) == _collidableEntities.cend())
        {
            _collidableEntities.push_back(entity->id);
        }
        return (true);
    }
    return (false);
}

bool    RenderingSystem::onEntityDeleted(Entity* entity)
{
    System::onEntityDeleted(entity);

    auto foundEntity = std::find(_collidableEntities.cbegin(), _collidableEntities.cend(), entity->id);
    // The entity is in the system list
    if (foundEntity != _collidableEntities.cend())
    {
        _collidableEntities.erase(foundEntity);
        return (true);
    }
    return (false);
}
