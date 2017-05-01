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
#include <Engine/Debug/Logger.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Graphics/UI/Font.hpp>
#include <Engine/Graphics/Geometries/Trapeze.hpp>
#include <Engine/Graphics/Renderer.hpp>

#include <Engine/Debug/LevelEntitiesDebugWindow.hpp>

#include <Engine/Systems/RenderingSystem.hpp>


bool RenderingSystem::_displayAllColliders = false;
std::unique_ptr<BufferPool> RenderingSystem::_bufferPool = nullptr;
std::unique_ptr<BufferPool> RenderingSystem::_batchesBufferPool = nullptr;

RenderingSystem::RenderingSystem(std::unordered_map<uint32_t, sEmitter*>* particleEmitters):
                                _particleEmitters(particleEmitters)
{
    addDependency<sRenderComponent>();

    if (!_bufferPool)
    {
        _bufferPool = std::make_unique<BufferPool>(50,
                                    static_cast<uint32_t>(sizeof(glm::mat4) + sizeof(glm::vec4)),
                                    GL_SHADER_STORAGE_BUFFER);
    }
    if (!_batchesBufferPool)
    {
        _batchesBufferPool = std::make_unique<BufferPool>(1,
                                    static_cast<uint32_t>(sizeof(glm::mat4) + sizeof(glm::vec4)) * INSTANCING_MAX,
                                    GL_SHADER_STORAGE_BUFFER);
    }
}

RenderingSystem::~RenderingSystem()
{
    for (auto& batch: _batches)
    {
        batch.buffer->free();
    }
}

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
            Geometry* boxModel = GeometryFactory::getGeometry(Geometry::eType::BOX);
            boxCollider->box = std::make_unique<ModelInstance>(boxModel);

            Material* colliderMaterial = ResourceManager::getInstance()->getResource<Material>("colliders.mat");
            boxCollider->box->setMaterial(colliderMaterial);
        }

        glm::mat4 boxTransform = transform->getTransform();
        boxTransform = glm::translate(boxTransform, boxCollider->pos);
        boxTransform = glm::scale(boxTransform, boxCollider->size);

        BufferPool::SubBuffer* buffer = boxCollider->box->getBuffer(_bufferPool.get());
        updateModelBuffer(buffer, boxTransform, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f));

        _renderQueue.addModel(boxCollider->box.get(), buffer->ubo, buffer->offset, buffer->size, 0, false, true);
    }
    if (sphereCollider && sphereCollider->display &&
        (LevelEntitiesDebugWindow::getSelectedEntityId() == entity->id || _displayAllColliders))
    {
        if (!sphereCollider->sphere)
        {
            Sphere::sInfo sphereInfo;
            sphereInfo.radius = SIZE_UNIT;
            Geometry* sphereModel = GeometryFactory::getGeometry(Geometry::eType::SPHERE);
            sphereCollider->sphere = std::make_unique<ModelInstance>(sphereModel);

            Material* colliderMaterial = ResourceManager::getInstance()->getResource<Material>("colliders.mat");
            sphereCollider->sphere->setMaterial(colliderMaterial);
        }

        glm::mat4 sphereTransform = transform->getTransform();
        sphereTransform = glm::translate(sphereTransform, sphereCollider->pos);
        sphereTransform = glm::scale(sphereTransform, glm::vec3(sphereCollider->radius));

        BufferPool::SubBuffer* buffer = sphereCollider->sphere->getBuffer(_bufferPool.get());
        updateModelBuffer(buffer, sphereTransform, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f));

        _renderQueue.addModel(sphereCollider->sphere.get(), buffer->ubo, buffer->offset, buffer->size, 0, false, true);
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

        sUiComponent* uiComponent = entity->getComponent<sUiComponent>();
        sRenderComponent *render = entity->getComponent<sRenderComponent>();
        sParticleEmitterComponent *emitterComp = entity->getComponent<sParticleEmitterComponent>();
        auto&& model = emitterComp->getModelInstance();

        if (!model || !render->_display)
            continue;

        // Update animation
        if (emitterComp->displayOnlyParticles && // If the sRenderComponent is also displayed, the animation will
                                                 // already be updated in sRenderComponent render loop
            render->_animator.isPlaying())
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

        if (uiComponent)
            _renderQueue.addUIModel(model,
                                    emitter->buffer->ubo,
                                    uiComponent->layer,
                                    0,
                                    emitter->buffer->size,
                                    emitter->particlesNb);
        else
            _renderQueue.addModel(model,
                                    emitter->buffer->ubo,
                                    0,
                                    emitter->buffer->size,
                                    emitter->particlesNb,
                                    false,
                                    true);
    }
}

void    RenderingSystem::addLightConeToRenderQueue(sLightComponent* lightComp, sTransformComponent* transform)
{
    if (!lightComp->_lightCone)
    {
        Geometry* coneModel = GeometryFactory::getGeometry(Geometry::eType::CONE);
        lightComp->_lightCone = std::make_unique<ModelInstance>(coneModel);

        Material* material = ResourceManager::getInstance()->getResource<Material>("light.mat");
        lightComp->_lightCone->setMaterial(material);
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


        glm::vec3 visibleNearSize = camera.getVisibleAreaSize({trapezeInfos.top.width,
                                                                trapezeInfos.top.length,
                                                                trapezeInfos.height});
        glm::vec3 visibleFarSize = camera.getVisibleAreaSize({trapezeInfos.bottom.width,
                                                                trapezeInfos.bottom.length,
                                                                trapezeInfos.height});


        transform->_posOffsetLocal = glm::vec3((trapezeInfos.bottom.width - visibleFarSize.x) / 2.0f * -1.0f,
                                                (trapezeInfos.bottom.length - visibleFarSize.y) / 2.0f * - 1.0f,
                                                0.0f);

        trapezeInfos.bottom.width = visibleFarSize.x;
        trapezeInfos.bottom.length = visibleFarSize.y;
        trapezeInfos.top.width = visibleNearSize.x;
        trapezeInfos.top.length = visibleNearSize.y;


        cameraComp->_cameraPerspective = std::make_unique<Trapeze>(trapezeInfos);
        cameraComp->_cameraView = std::make_unique<ModelInstance>(cameraComp->_cameraPerspective.get());

        cameraComp->_cameraView->setMaterial(material);
    }

    transform->_posOffsetWorld = glm::vec3(0.0f);

    glm::mat4 transformMat = transform->getTransform();

    // Rotate trapeze so that top and bottom faces are parallel to camera direction
    glm::quat rotation(glm::vec3(glm::radians(90.0f), glm::radians(0.0f), glm::radians(0.0f)));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -transform->_posOffsetLocal);
    // Place the camera to the back of the trapeze
    translate = glm::translate(translate, glm::vec3(0.0f,
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
        Geometry* viewPreviewModel = GeometryFactory::getGeometry(Geometry::eType::BOX);
        cameraComp->_cameraView = std::make_unique<ModelInstance>(viewPreviewModel);

        Material* material = ResourceManager::getInstance()->getResource<Material>("camera.mat");
        cameraComp->_cameraView->setMaterial(material);
    }

    glm::mat4 transformMat = transform->getTransform();

    glm::vec3 projSize;
    projSize.x = camera.getProjSize() * camera.getAspect();
    projSize.y = camera.getProjSize();
    projSize.z = camera.getFar() - camera.getNear();
    glm::vec3 visibleProjSize = camera.getVisibleAreaSize(projSize);

    transform->_posOffsetLocal = glm::vec3((projSize.x - visibleProjSize.x) / 2.0f * -1.0f,
                                            (projSize.y - visibleProjSize.y) / 2.0f * - 1.0f,
                                            0.0f);
    transform->_posOffsetWorld = glm::vec3(0.0f);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), transform->_posOffsetLocal);
    // Place the camera to the back of the box
    translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -(visibleProjSize.z / 2.0f)));
    // Translate camera near
    translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -camera.getNear()));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(visibleProjSize.x / SIZE_UNIT,
                                                        visibleProjSize.y / SIZE_UNIT,
                                                        visibleProjSize.z / SIZE_UNIT));
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

    for (auto& batch: _batches)
    {
        batch.buffer->free();
    }
    _batches.clear();
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    #if defined(ENGINE_DEBUG)
    if (keyboard.getStateMap()[Keyboard::eKey::C] == Keyboard::eKeyState::KEY_PRESSED)
        _displayAllColliders = !_displayAllColliders;
    #endif

    forEachEntity(em, [&](Entity *entity) {
        sParticleEmitterComponent* particleEmitterComp = entity->getComponent<sParticleEmitterComponent>();
        // Display the sRenderComponent only if there is no sParticleEmitterComponent
        // Or if the user want to render both sRenderComponent and sParticleEmitterComponent
        if (!particleEmitterComp || !particleEmitterComp->displayOnlyParticles)
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

                sUiComponent* uiComponent = entity->getComponent<sUiComponent>();
                sTextComponent* textComponent = entity->getComponent<sTextComponent>();

                if (uiComponent)
                {
                    BufferPool::SubBuffer* buffer = getModelBuffer(transform, render);
                    _renderQueue.addUIModel(model, buffer->ubo, uiComponent->layer, buffer->offset, buffer->size);
                }
                else
                {
                    addBatch(transform, render);
                }

                if (textComponent)
                {
                    _renderQueue.addText(textComponent->text,
                                        uiComponent ? uiComponent->layer : 0,
                                        glm::vec2(transform->getPos().x, transform->getPos().y) + textComponent->offset);
                }

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

   for (auto& batch: _batches)
   {
        _renderQueue.addMesh(batch.meshInstance, batch.buffer->ubo, 0, batch.buffer->size, batch.instances, batch.dynamic, batch.hideDynamic);
   }

    // Add cameras views to render queue
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

            if (!_camera)
            {
                Renderer::getInstance()->render(&cameraComp->camera, _renderQueue);
            }
        }

        if (_camera)
        {
            #if defined(ENGINE_DEBUG)
                uint32_t selectedEntityId = LevelEntitiesDebugWindow::getSelectedEntityId();
                Entity* selectedEntity = em.getEntity(selectedEntityId);
                if (selectedEntity)
                {
                    sCameraComponent* cameraComp = selectedEntity->getComponent<sCameraComponent>();
                    sTransformComponent* transform = selectedEntity->getComponent<sTransformComponent>();
                    if (cameraComp)
                    {
                        addCameraViewToRenderQueue(cameraComp, transform);
                    }
                }
            #endif
            Renderer::getInstance()->render(_camera, _renderQueue);
        }
        // There is no camera but we can still render UI
        else if (cameras.size() == 0)
        {
            Renderer::getInstance()->render(nullptr, _renderQueue);
        }
    }
}

void    RenderingSystem::addBatch(sTransformComponent* transform, sRenderComponent* render)
{
    auto&& model = render->getModelInstance();
    auto& meshsInstances = model->getMeshsInstances();
    for (auto& meshInstance: meshsInstances)
    {
        auto material = meshInstance->getMaterial();
        auto mesh = meshInstance->getMesh();
        bool dynamic = render->dynamic;
        bool hideDynamic = render->hideDynamic;
        sBatch* batch = nullptr;
        for (auto& batch_: _batches)
        {
            if (batch_.mesh == mesh &&
                batch_.material == material &&
                batch_.dynamic == dynamic &&
                batch_.hideDynamic == hideDynamic &&
                batch_.instances < INSTANCING_MAX)
            {
                batch = &batch_;
                break;
            }
        }
        if (batch == nullptr)
        {
            _batches.push_back({});
            batch = &_batches.back();
            batch->buffer = _batchesBufferPool->allocate();
            batch->material = material;
            batch->mesh = mesh;
            batch->meshInstance = meshInstance.get();
            batch->dynamic = dynamic;
            batch->hideDynamic = hideDynamic;
        }
        uint32_t offset = batch->instances * (sizeof(glm::vec4) + sizeof(glm::mat4));
        auto transform_ = transform->getTransform();
        batch->buffer->ubo->update((void*)&transform_, sizeof(glm::mat4), offset);
        batch->buffer->ubo->update((void*)&render->color, sizeof(glm::vec4), offset + sizeof(glm::mat4));
        batch->instances++;
    }
}

BufferPool::SubBuffer*  RenderingSystem::getModelBuffer(sTransformComponent* transform,sRenderComponent* render)
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
