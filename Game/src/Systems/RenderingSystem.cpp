#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Utils/Debug.hpp"
#include "Utils/Exception.hpp"
#include "Utils/RessourceManager.hpp"
#include "Window/GameWindow.hpp"

#include "Systems/RenderingSystem.hpp"


RenderingSystem::RenderingSystem(Map* map, std::unordered_map<uint32_t, sEmitter*>* particleEmitters): _map(map), _particleEmitters(particleEmitters)
{

    addDependency<sPositionComponent>();
    addDependency<sRenderComponent>();
}

RenderingSystem::~RenderingSystem() {}

bool    RenderingSystem::init()
{
    try
    {
        _shaderProgram.attachShader(GL_VERTEX_SHADER, "ressources/shaders/shader.vert");
        _shaderProgram.attachShader(GL_FRAGMENT_SHADER, "ressources/shaders/shader.frag");
        _shaderProgram.link();
        _shaderProgram.use();
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    // Enable blend for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
}

void    RenderingSystem::renderParticles(EntityManager& em)
{
    // Activate additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (auto &&it: *_particleEmitters)
    {
        auto &&emitter = it.second;
        Entity* entity = em.getEntity(it.first);
        sRenderComponent *sprite = entity->getComponent<sRenderComponent>();


        if (!sprite->_sprite)
        {
            Sprite::sCreateInfo createInfo;
            sprite->_sprite = new Sprite(sprite->type, _shaderProgram);

            getSpriteCreateInfo(createInfo, sprite);
            sprite->_sprite->loadFromTexture(createInfo);
        }

        for (unsigned int i = 0; i < emitter->particlesNb; i++)
        {
            auto &&particle = emitter->particles[i];

            // Model matrice
            glm::mat4 trans;
            glm::mat4 scale;
            glm::mat4 modelTrans;
            trans = glm::translate(trans, glm::vec3(particle.pos.x, particle.pos.y, 0.0f));
            scale = glm::scale(scale, glm::vec3(particle.size, particle.size, particle.size));
            modelTrans = trans * scale;
            GLint uniTrans = _shaderProgram.getUniformLocation("trans");
            glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(modelTrans));

            // Color vector
            GLint uniColor = _shaderProgram.getUniformLocation("color");
            glUniform4f(uniColor, particle.color.x, particle.color.y, particle.color.z, particle.color.w);

            // Draw sprite
            sprite->_sprite->draw();
        }
    }

    // Activate transparency blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
*/
void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    // Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT);

    // Projection matrix
    glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    GLint uniProj = _shaderProgram.getUniformLocation("proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    // View matrix
    glm::mat4 view = glm::lookAt(glm::vec3(-50.0f, 150.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    GLint uniView = _shaderProgram.getUniformLocation("view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    // Iterate over particle emitters
    forEachEntity(em, [&](Entity *entity) {
        renderEntity(entity);
    });

/*
    for (auto &&id: (*_map)[1].getEntities())
    {
        sPositionComponent *position;
        Entity* entity = em.getEntity(id);
        position = entity->getComponent<sPositionComponent>();
    }

    for (uint16_t layer = 0; layer < _map->getLayersNb(); layer++)
    {
        // Order the entities to properly render !
        // If not, the render will not work
        (*_map)[layer].orderEntities(em);
        auto &&it = (*_map)[layer].getEntities().cbegin();
        for (uint32_t y = 0; y < _map->getHeight(); y++)
        {
            for (uint32_t x = 0; x < _map->getWidth(); x++)
            {
                // Orthogonal projection matrice
                glm::mat4 ortho = glm::ortho(0.0f, (float)GameWindow::getInstance()->getScreenWidth() * 1.3f, 0.0f, (float)GameWindow::getInstance()->getScreenHeight() * 1.3f);
                GLint uniProj = _shaderProgram.getUniformLocation("proj");
                glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(ortho));

                Entity* tile = em.getEntity((*_map)[layer][y][x].get());

                if (tile)
                    renderEntity(tile);

                renderEntities(em, it, layer, x, y);
            }
        }
    }*/

    //renderParticles(em);

    // Display imgui windows
    //ImGui::Render();

    // Display screen
    GameWindow::getInstance()->display();
}

std::shared_ptr<Model>  RenderingSystem::getModel(Entity* entity)
{
    int id = entity->id;
    sRenderComponent *model = entity->getComponent<sRenderComponent>();
/*    sPositionComponent *position = entity->getComponent<sPositionComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();*/

    // The entity does not exist in the render system
    if (!model->_model)
    {
        model->_model = RessourceManager::getInstance()->getModel(model->modelFile);
    }

    return (model->_model);
}

const ShaderProgram&  RenderingSystem::getShaderProgram() const
{
    return _shaderProgram;
}
