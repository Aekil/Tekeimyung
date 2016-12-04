#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Game/EntityDebugWindow.hpp>

#include <Game/Systems/RenderingSystem.hpp>


RenderingSystem::RenderingSystem(Map* map, std::unordered_map<uint32_t, sEmitter*>* particleEmitters): _map(map), _particleEmitters(particleEmitters)
{
    addDependency<sRenderComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(RENDERING_SYSTEM_NAME);

    _camera.translate(glm::vec3(350.0f, 250.0f, 300.0f));
    _camera.setDir(glm::vec3(-30.0f));

    // Set camera screen
    float size = 500.0f;
    Camera::sScreen screen;
    screen.right = size * _camera.getAspect();
    screen.left = -screen.right;
    screen.top = size;
    screen.bottom = -screen.top;
    _camera.setScreen(screen);

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

        const_cast<UniformBuffer&>(_camera.getUbo()).bind(_shaderProgram, "camera");
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

void    RenderingSystem::renderEntity(sRenderComponent *render, Entity* entity, float elapsedTime)
{
    sTransformComponent *transform = entity->getComponent<sTransformComponent>();
    auto&& model = getModel(render);

    // Update animation
    if (render->_animator.isPlaying())
    {
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        render->_animator.update(elapsedTime);
        transform->needUpdate = true;
    }

    // Draw model
    model->draw(_shaderProgram, render->color, transform->getTransform());
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
            Box::sInfo boxInfo;
            boxInfo.width = boxCollider->size.x;
            boxInfo.height = boxCollider->size.y;
            boxInfo.length = boxCollider->size.z;
            boxCollider->box = std::make_shared<Box>(boxInfo);
        }

        glm::mat4 boxTransform = transform->transform;
        boxTransform = glm::translate(boxTransform, boxCollider->pos);
        boxTransform = glm::scale(boxTransform, boxCollider->size);

        boxCollider->box->draw(_shaderProgram, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), boxTransform);
    }
    if (sphereCollider && sphereCollider->display)
    {
        if (!sphereCollider->sphere)
        {
            Sphere::sInfo sphereInfo;
            sphereInfo.radius = sphereCollider->radius;
            sphereCollider->sphere = std::make_shared<Sphere>(sphereInfo);
        }

        glm::mat4 sphereTransform = transform->transform;
        sphereTransform = glm::translate(sphereTransform, sphereCollider->pos);
        sphereTransform = glm::scale(sphereTransform, glm::vec3(sphereCollider->radius));

        sphereCollider->sphere->draw(_shaderProgram, glm::vec4(0.87f, 1.0f, 1.0f, 0.1f), sphereTransform);
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

    renderCollider(em.getEntity(EntityDebugWindow::getSelectedEntityId()));
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
        auto&& model = getModel(render);

        // Only freeze camera rotation for planes
        if (render->type == Geometry::eType::PLANE)
            _camera.freezeRotations(true);

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 transformMatrix(1.0f);
            transformMatrix = glm::translate(transformMatrix, glm::vec3(particle.pos.x, particle.pos.y, particle.pos.z));
            transformMatrix = glm::scale(transformMatrix, particle.size);

            // Draw sprite
            model->draw(_shaderProgram, particle.color, transformMatrix);
        }

        _camera.freezeRotations(false);
    }

    // Activate transparency blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    Timer timer;
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
                sRenderComponent *render = entity->getComponent<sRenderComponent>();
                renderEntity(render, entity, elapsedTime);
                ++it;
            }
        }
    }


    renderColliders(em);

    renderParticles(em);

    // Enable depth buffer for opaque objects
    glDepthMask(GL_TRUE);
    // Disable blending for opaque objects
    glDisable(GL_BLEND);

    // Display imgui windows
    ImGui::Render();

    // Display screen
    GameWindow::getInstance()->display();

    _monitoringData.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, _monitoringData);
}

bool    RenderingSystem::isTransparent(sRenderComponent *render) const
{

    return (render->type == Geometry::eType::PLANE);
}

std::shared_ptr<Model>  RenderingSystem::getModel(sRenderComponent *render)
{
    // The entity does not exist in the render system
    if (!render->_model)
    {
        render->initModel();
    }

    return (render->_model);
}

const ShaderProgram&  RenderingSystem::getShaderProgram() const
{
    return _shaderProgram;
}

void    RenderingSystem::onEntityNewComponent(Entity* entity, sComponent* component)
{
    System::onEntityNewComponent(entity, component);

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
    }
}

void    RenderingSystem::onEntityRemovedComponent(Entity* entity, sComponent* component)
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
        }
    }
}

void    RenderingSystem::onEntityDeleted(Entity* entity)
{
    System::onEntityDeleted(entity);

    auto foundEntity = std::find(_collidableEntities.cbegin(), _collidableEntities.cend(), entity->id);
    // The entity is in the system list
    if (foundEntity != _collidableEntities.cend())
    {
        _collidableEntities.erase(foundEntity);
    }
}
