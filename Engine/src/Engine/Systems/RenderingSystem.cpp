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
#include <Engine/Graphics/Geometries/Trapeze.hpp>
#include <Engine/Graphics/Renderer.hpp>

#include <Engine/LevelEntitiesDebugWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>


bool RenderingSystem::_displayAllColliders = false;
std::unique_ptr<BufferPool> RenderingSystem::_bufferPool = nullptr;

RenderingSystem::RenderingSystem(std::unordered_map<uint32_t, sEmitter*>* particleEmitters):
                                _particleEmitters(particleEmitters)
{
    addDependency<sRenderComponent>();

    if (!_bufferPool)
    {
        _bufferPool = std::make_unique<BufferPool>(50,
                                    (uint32_t)(sizeof(glm::mat4) + sizeof(glm::vec4)),
                                    GL_SHADER_STORAGE_BUFFER);
    }
}

RenderingSystem::~RenderingSystem() {}

void    RenderingSystem::attachCamera(Camera* camera)
{
    _camera = camera;
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

        BufferPool::SubBuffer* buffer = boxCollider->box->getBuffer(_bufferPool.get());
        updateModelBuffer(buffer, boxTransform, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f));

        _renderQueue.addModel(boxCollider->box.get(), buffer->ubo, buffer->offset, buffer->size);
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

        BufferPool::SubBuffer* buffer = sphereCollider->sphere->getBuffer(_bufferPool.get());
        updateModelBuffer(buffer, sphereTransform, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f));

        _renderQueue.addModel(sphereCollider->sphere.get(), buffer->ubo, buffer->offset, buffer->size);
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
            transform->needUpdate();
        }

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(particle.pos.x, particle.pos.y, particle.pos.z));
            transformMatrix = glm::scale(transformMatrix, particle.size);

            uint32_t uboOffset = (sizeof(glm::mat4) + sizeof(glm::vec4)) * i;
            emitter->buffer->ubo->update(&transformMatrix, sizeof(glm::mat4), uboOffset);
            emitter->buffer->ubo->update(&particle.color, sizeof(glm::vec4), uboOffset + sizeof(glm::mat4));

        }

        if (isUi)
            _renderQueue.addUIModel(model, emitter->buffer->ubo, 0, emitter->buffer->size, emitter->particlesNb);
        else
            _renderQueue.addModel(model, emitter->buffer->ubo, 0, emitter->buffer->size, emitter->particlesNb);
    }
}

void    RenderingSystem::addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform)
{
    if (!lightComp->_lightCone)
    {
        Material* material = ResourceManager::getInstance()->getResource<Material>("light.mat");
        Geometry* coneModel = GeometryFactory::getGeometry(Geometry::eType::CONE);

        coneModel->setMaterial(material);
        lightComp->_lightCone = std::make_unique<ModelInstance>(coneModel);
    }

    BufferPool::SubBuffer* buffer = lightComp->_lightCone->getBuffer(_bufferPool.get());
    updateModelBuffer(buffer, transform->getTransform(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    _renderQueue.addModel(lightComp->_lightCone.get(), buffer->ubo, buffer->offset, buffer->size);
}

void    RenderingSystem::addCameraViewPerspectiveToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform)
{
    Camera& camera = cameraComp->camera;
    if (!cameraComp->_cameraView)
    {
        Material* material = ResourceManager::getInstance()->getResource<Material>("camera.mat");
        float aspect = camera.getAspect();
        float fov = glm::radians(camera.getFov());

        Trapeze::sInfo trapezeInfos;
        trapezeInfos.height = camera.getFar() - camera.getNear();

        // Near plane size
        trapezeInfos.top.length = 2.0f * tan(fov / 2.0f) * camera.getNear();
        trapezeInfos.top.width = trapezeInfos.top.length * aspect;

        // Far plane size
        trapezeInfos.bottom.length = 2.0f * tan(fov / 2.0f) * camera.getFar();
        trapezeInfos.bottom.width = trapezeInfos.bottom.length * aspect;

        cameraComp->_cameraPerspective = std::make_unique<Trapeze>(trapezeInfos);

        cameraComp->_cameraPerspective->setMaterial(material);
        cameraComp->_cameraView = std::make_unique<ModelInstance>(cameraComp->_cameraPerspective.get());
    }

    transform->_posOffsetLocal = glm::vec3(0.0f);
    transform->_posOffsetWorld = glm::vec3(0.0f);

    glm::mat4 transformMat = transform->getTransform();

    // Rotate trapeze so that top and bottom faces are parallel to camera direction
    glm::quat rotation(glm::vec3(glm::radians(90.0f), glm::radians(0.0f), glm::radians(0.0f)));
    // Place the camera to the back of the trapeze
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,
                                                            0.0f,
                                                            -((camera.getFar() - camera.getNear()) / 2.0f)));
    // Translate camera near
    translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -camera.getNear()));

    transformMat = transformMat * translate * glm::mat4_cast(rotation);



    BufferPool::SubBuffer* buffer = cameraComp->_cameraView->getBuffer(_bufferPool.get());
    updateModelBuffer(buffer, transformMat, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    _renderQueue.addModel(cameraComp->_cameraView.get(), buffer->ubo, buffer->offset, buffer->size);
}

void    RenderingSystem::addCameraViewOrthoGraphicToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform)
{
    Camera& camera = cameraComp->camera;
    if (!cameraComp->_cameraView)
    {
        Material* material = ResourceManager::getInstance()->getResource<Material>("camera.mat");
        Geometry* viewPreviewModel = GeometryFactory::getGeometry(Geometry::eType::BOX);
        viewPreviewModel->setMaterial(material);
        cameraComp->_cameraView = std::make_unique<ModelInstance>(viewPreviewModel);
    }

    glm::mat4 transformMat = transform->getTransform();

    glm::vec3 projSize;
    projSize.x = camera.getProjSize() * camera.getAspect();
    projSize.y = camera.getProjSize();
    projSize.z = camera.getFar() - camera.getNear();

    // Translate box corresponding to viewport offsets and near distance
    transform->_posOffsetLocal = glm::vec3(camera.getViewport().offset.x,
                            camera.getViewport().offset.y,
                            0.0f);
    transform->_posOffsetWorld = glm::vec3(0.0f);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), transform->_posOffsetLocal);
    // Place the camera to the back of the box
    translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -(projSize.z / 2.0f)));
    // Translate camera near
    translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -camera.getNear()));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(projSize.x / SIZE_UNIT,
                                                        projSize.y / SIZE_UNIT,
                                                        projSize.z / SIZE_UNIT));
    transformMat = transformMat * translate * scale;

    BufferPool::SubBuffer* buffer = cameraComp->_cameraView->getBuffer(_bufferPool.get());
    updateModelBuffer(buffer, transformMat, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    _renderQueue.addModel(cameraComp->_cameraView.get(), buffer->ubo, buffer->offset, buffer->size);

}

void    RenderingSystem::addCameraViewToRenderQueue(sCameraComponent* cameraComp, sTransformComponent* transform)
{
    if (cameraComp->camera.getProjType() == Camera::eProj::PERSPECTIVE)
    {
        addCameraViewPerspectiveToRenderQueue(cameraComp, transform);
    }
    else if (cameraComp->camera.getProjType() == Camera::eProj::ORTHOGRAPHIC_3D)
    {
        addCameraViewOrthoGraphicToRenderQueue(cameraComp, transform);
    }
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
                    transform->needUpdate();
                }

                BufferPool::SubBuffer* buffer = getModelBuffer(transform, render);

                if (entity->hasComponent<sUiComponent>())
                    _renderQueue.addUIModel(model, buffer->ubo, buffer->offset, buffer->size);
                else
                    _renderQueue.addModel(model, buffer->ubo, buffer->offset, buffer->size);

                addCollidersToRenderQueue(entity, transform);
            }
        }
    });

    addParticlesToRenderQueue(em, elapsedTime);

    // Add lights to render queue
    {
        auto& lights = em.getEntitiesByComponent<sLightComponent>();
        for (Entity* light : lights)
        {
            sLightComponent* lightComp = light->getComponent<sLightComponent>();
            sTransformComponent* transform = light->getComponent<sTransformComponent>();

            // Only display light cone in debug mode
            #if defined(ENGINE_DEBUG)
                if (LevelEntitiesDebugWindow::getSelectedEntityId() == light->id)
                {
                    addLightConeToRenderQueue(lightComp, transform);
                }
            #endif

            // Update light direction depending on rotation
            if (transform->getRotation() != lightComp->lastRotation)
            {
                lightComp->lastRotation = transform->getRotation();
                glm::quat rotation(glm::vec3(glm::radians(transform->getRotation().x), glm::radians(transform->getRotation().y), glm::radians(transform->getRotation().z)));
                lightComp->light.setDirection(glm::vec3(glm::mat4_cast(rotation) * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
            }

            _renderQueue.addLight(&lightComp->light);
        }
    }

    // Get scene camera
    Camera* camera = nullptr;
    {
        // Use attached camera if exists
        // Else use the camera of camera entities
        if (_camera)
        {
            camera = _camera;
        }
        else
        {
            auto& cameras = em.getEntitiesByComponent<sCameraComponent>();
            // Render with the first camera found
            // TODO: render with multiple camera ?
            if (cameras.size() > 0)
            {
                camera = &cameras[0]->getComponent<sCameraComponent>()->camera;
            }
        }
    }

    // Add cameras views to render queue
    #if defined(ENGINE_DEBUG)
    {
        auto& cameras = em.getEntitiesByComponent<sCameraComponent>();
        for (auto& camera: cameras)
        {
            sTransformComponent* transform = camera->getComponent<sTransformComponent>();
            sCameraComponent* cameraComp = camera->getComponent<sCameraComponent>();

            // Update camera transform
            if (transform->isDirty())
            {
                transform->isDirty(false);
                cameraComp->camera.setRotation(transform->getRotation());
                cameraComp->camera.setPos(transform->getPos());
            }

            if (LevelEntitiesDebugWindow::getSelectedEntityId() == camera->id)
            {
                addCameraViewToRenderQueue(cameraComp, transform);
            }
        }
    }
    #endif

    Renderer::getInstance()->render(camera, _renderQueue);
}

BufferPool::SubBuffer*  RenderingSystem::getModelBuffer(sTransformComponent* transform, sRenderComponent* render)
{
    BufferPool::SubBuffer* buffer = render->getModelInstance()->getBuffer(_bufferPool.get());

    if (transform->isDirty() || render->lastColor != render->color)
    {
        transform->isDirty(false);
        render->lastColor = render->color;
        updateModelBuffer(buffer, transform->getTransform(), render->color);
    }

    return (buffer);
}

void    RenderingSystem::updateModelBuffer(BufferPool::SubBuffer* buffer, const glm::mat4& transform, const glm::vec4& color)
{
    buffer->ubo->update((void*)&transform, sizeof(glm::mat4), buffer->offset);
    buffer->ubo->update((void*)&color, sizeof(glm::vec4), buffer->offset + sizeof(glm::mat4));
}
