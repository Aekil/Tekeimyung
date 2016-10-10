#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/GameWindow.hpp"
#include "Utils/Exception.hpp"

RenderingSystem::RenderingSystem(Map* map): _map(map)
{
    addDependency<sPositionComponent>();
    addDependency<sRenderComponent>();
}

RenderingSystem::~RenderingSystem()
{
    for (auto &&renderEntity: _renderEntities)
    {
        delete renderEntity.second;
    }
}

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

void    RenderingSystem::drawSquare(EntityManager& em, uint16_t layer, uint32_t x, uint32_t y, Map::eObjType type)
{
    Entity* entity = em.getEntity((*_map)[layer][y][x].get(type));

    if (!entity)
        return;

    Sprite* sprite = getSprite(entity);

    // Model matrice
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(sprite->getPos().x, sprite->getPos().y, 0.0f));
    GLint uniTrans = _shaderProgram.getUniformLocation("trans");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

    // Draw sprite
    sprite->draw();
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    // Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT);
    CollisionMap* collisionMap = _map->getCollisionMap();

    for (uint16_t layer = 0; layer < _map->getLayersNb(); layer++)
    {
        for (uint32_t y = 0; y < _map->getHeight(); y++)
        {
            for (uint32_t x = 0; x < _map->getWidth(); x++)
            {
                // Orthogonal projection matrice
                glm::mat4 ortho = glm::ortho(0.0f, (float)GameWindow::getInstance()->getWidth(), 0.0f, (float)GameWindow::getInstance()->getHeight());
                GLint uniProj = _shaderProgram.getUniformLocation("proj");
                glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(ortho));

                drawSquare(em, layer, x, y, Map::eObjType::STATIC);
                drawSquare(em, layer, x, y, Map::eObjType::DYNAMIC);
            }
        }
    }


    // Display screen
    GameWindow::getInstance()->display();
}

Sprite*   RenderingSystem::getSprite(Entity* entity)
{
    int id = entity->id;
    sRenderComponent *sprite = entity->getComponent<sRenderComponent>();
    sPositionComponent *position = entity->getComponent<sPositionComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
    sInputComponent *input = entity->getComponent<sInputComponent>();

    // The entity does not exist in the render system
    if (_renderEntities.find(id) == _renderEntities.end())
    {
        _renderEntities[id] = new Sprite(sprite->type, _shaderProgram);
        _renderEntities[id]->loadFromTexture(sprite->texture, sprite->animated, sprite->nbFrames, sprite->orientations, sprite->spriteSize);

    }

    // Update entity graphic position
    bool keyPressed = input && input->keyPressed;
    eOrientation orientation = direction ? direction->orientation : eOrientation::S;
    _renderEntities[id]->update(position->value, position->z, keyPressed, orientation);

    return (_renderEntities[id]);
}
