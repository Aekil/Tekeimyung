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


bool RenderingSystem::_displayAllColliders = false;

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

void    RenderingSystem::addCollidersToRenderQueue(Entity* entity, sTransformComponent* transform)
{
    if (!entity)
        return;

    sBoxColliderComponent* boxCollider = entity->getComponent<sBoxColliderComponent>();
    sSphereColliderComponent* sphereCollider = entity->getComponent<sSphereColliderComponent>();
    if (boxCollider && boxCollider->display &&
        (LevelEntitiesDebugWindow::getSelectedEntityId() == entity->id || _displayAllColliders))
    {
        if (!boxCollider->box)
        {
            Material* colliderMaterial = ResourceManager::getInstance()->getResource<Material>("colliders.mat");
            Geometry* boxModel = GeometryFactory::getGeometry(Geometry::eType::BOX);

            boxModel->setMaterial(colliderMaterial);
            boxCollider->box = std::make_unique<ModelInstance>(boxModel);
        }

        glm::mat4 boxTransform = transform->getTransform();
        boxTransform = glm::translate(boxTransform, boxCollider->pos);
        boxTransform = glm::scale(boxTransform, boxCollider->size);

        _renderQueue.addModel(boxCollider->box.get(), glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), boxTransform);
    }
    if (sphereCollider && sphereCollider->display &&
        (LevelEntitiesDebugWindow::getSelectedEntityId() == entity->id || _displayAllColliders))
    {
        if (!sphereCollider->sphere)
        {
            Sphere::sInfo sphereInfo;
            sphereInfo.radius = SIZE_UNIT;
            Material* colliderMaterial = ResourceManager::getInstance()->getResource<Material>("colliders.mat");
            Geometry* sphereModel = GeometryFactory::getGeometry(Geometry::eType::SPHERE);

            sphereModel->setMaterial(colliderMaterial);
            sphereCollider->sphere = std::make_unique<ModelInstance>(sphereModel);
        }

        glm::mat4 sphereTransform = transform->getTransform();
        sphereTransform = glm::translate(sphereTransform, sphereCollider->pos);
        sphereTransform = glm::scale(sphereTransform, glm::vec3(sphereCollider->radius));

        _renderQueue.addModel(sphereCollider->sphere.get(), glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), sphereTransform);
    }
}

void    RenderingSystem::addParticlesToRenderQueue(EntityManager& em, float elapsedTime)
{
    if (!_particleEmitters)
        return;

    for (auto &&it: *_particleEmitters)
    {
        auto &&emitter = it.second;
        Entity* entity = em.getEntity(it.first);
        if (entity == nullptr)
            continue;

        bool isUi = entity->hasComponent<sUiComponent>();
        sRenderComponent *render = entity->getComponent<sRenderComponent>();
        auto&& model = render->getModelInstance();

        if (!model || !render->_display)
            continue;

        // Update animation
        if (render->_animator.isPlaying())
        {
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();

            render->_animator.update(elapsedTime);
            transform->needUpdate = true;
        }

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(particle.pos.x, particle.pos.y, particle.pos.z));
            transformMatrix = glm::scale(transformMatrix, particle.size);

            if (isUi)
                _renderQueue.addUIModel(model, particle.color, transformMatrix);
            else
            _renderQueue.addModel(model, particle.color, transformMatrix);
        }
    }
}

void    RenderingSystem::addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform)
{
    if (!lightComp->_lightCone)
    {
        //Material* colliderMaterial = ResourceManager::getInstance()->getResource<Material>("colliders.mat");
        Geometry* coneModel = GeometryFactory::getGeometry(Geometry::eType::CONE);

        //boxModel->setMaterial(colliderMaterial);
        lightComp->_lightCone = std::make_unique<ModelInstance>(coneModel);
    }

    //glm::mat4 coneTransform = transform->getTransform();
    //coneTransform = glm::scale(coneTransform, glm::vec3(3.0f, 3.0f, 3.0f));

    _renderQueue.addModel(lightComp->_lightCone.get(), glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), transform->getTransform());
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
   _renderQueue.clear();

    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    if (keyboard.getStateMap()[Keyboard::eKey::C] == Keyboard::eKeyState::KEY_PRESSED)
        _displayAllColliders = !_displayAllColliders;

    forEachEntity(em, [&](Entity *entity) {
        // Don't display particle systems
        if (!entity->getComponent<sParticleEmitterComponent>())
        {
            sRenderComponent *render = entity->getComponent<sRenderComponent>();
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();
            auto&& model = render->getModelInstance();

            if (model && render->_display)
            {
                // Update animation
                if (render->_animator.isPlaying())
                {
                    render->_animator.update(elapsedTime);
                    transform->needUpdate = true;
                }

                if (entity->hasComponent<sUiComponent>())
                    _renderQueue.addUIModel(model, render->color, transform->getTransform());
                else
                    _renderQueue.addModel(model, render->color, transform->getTransform());

                addCollidersToRenderQueue(entity, transform);
            }
        }
    });

    addParticlesToRenderQueue(em, elapsedTime);

    // Add lights to render queue
    {
        auto& entities = em.getEntitiesByComponent<sLightComponent>();
        for (Entity* entity : entities)
        {
            sLightComponent* lightComp = entity->getComponent<sLightComponent>();
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();

            // Only display light cone in debug mode
            #if defined(ENGINE_DEBUG)
                addLightConeToRenderQueue(lightComp, transform);
            #endif

            // Update light direction depending on rotation
            if (transform->rotation != lightComp->lastRotation)
            {
                lightComp->lastRotation = transform->rotation;
                glm::quat rotation(glm::vec3(glm::radians(transform->rotation.x), glm::radians(transform->rotation.y), glm::radians(transform->rotation.z)));
                lightComp->light.setDirection(glm::vec3(glm::mat4_cast(rotation) * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
            }

            _renderQueue.addLight(&lightComp->light);
        }
    }

    Renderer::getInstance()->render(_camera, _renderQueue);
}
