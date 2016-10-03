#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/GameWindow.hpp"
#include "Utils/EngineException.hpp"

RenderingSystem::RenderingSystem()
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
    catch(const EngineException& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    // Enable blend for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return (true);
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{
    // Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT);

    forEachEntity(em, [&](Entity *entity) {
        Sprite* sprite = getSprite(entity);
        sPositionComponent *position = entity->getComponent<sPositionComponent>();

        // Orthogonal projection matrice
        glm::mat4 ortho = glm::ortho(0.0f, (float)GameWindow::getInstance()->getWidth(), 0.0f, (float)GameWindow::getInstance()->getHeight());
        GLint uniProj = _shaderProgram.getUniformLocation("proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(ortho));

        // Model matrice
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(sprite->getPos().x, sprite->getPos().y, 0.0f));
        GLint uniTrans = _shaderProgram.getUniformLocation("trans");
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        // Draw sprite
        sprite->draw();
    });


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
    _renderEntities[id]->update(position->x, position->y, position->z, keyPressed, orientation);

    return (_renderEntities[id]);
}
