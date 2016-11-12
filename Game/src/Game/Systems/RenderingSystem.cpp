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
    Sprite* sprite = getSprite(entity);

    // Model matrice
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(sprite->getPos().x, sprite->getPos().y, 0.0f));
    GLint uniTrans = _shaderProgram.getUniformLocation("trans");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

    // Color vector
    GLint uniColor = _shaderProgram.getUniformLocation("color");
    glUniform4f(uniColor, sprite->getColor().x, sprite->getColor().y, sprite->getColor().z, 1.0f);

    // Draw sprite
    sprite->draw();
}

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

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    // Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT);

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
    }

    renderParticles(em);

    // Display imgui windows
    ImGui::Render();

    // Display screen
    GameWindow::getInstance()->display();
}

Sprite*   RenderingSystem::getSprite(Entity* entity)
{
    int id = entity->id;
    sRenderComponent *sprite = entity->getComponent<sRenderComponent>();
    sPositionComponent *position = entity->getComponent<sPositionComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

    // The entity does not exist in the render system
    if (!sprite->_sprite)
    {
        Sprite::sCreateInfo createInfo;
        sprite->_sprite = new Sprite(sprite->type, _shaderProgram);

        getSpriteCreateInfo(createInfo, sprite);
        sprite->_sprite->loadFromTexture(createInfo);

    }

    // Update entity graphic position
    bool moved = direction && direction->moved;
    eOrientation orientation = direction ? direction->orientation : eOrientation::N;
    sprite->_sprite->update(position->value, position->z, moved, orientation, sprite->color);

    return (sprite->_sprite);
}

void    RenderingSystem::getSpriteCreateInfo(Sprite::sCreateInfo& createInfo, sRenderComponent *sprite)
{
    createInfo.textureFile = sprite->texture;
    createInfo.animated = sprite->animated;
    createInfo.frames = sprite->frames;
    createInfo.offset = sprite->spriteSheetOffset;
    createInfo.orientations = sprite->orientations;
    createInfo.spriteSize = sprite->spriteSize;
    createInfo.color = sprite->color;
}

const ShaderProgram&  RenderingSystem::getShaderProgram() const
{
    return _shaderProgram;
}
