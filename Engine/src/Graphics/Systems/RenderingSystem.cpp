#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/GameWindow.hpp"
#include "Utils/EngineException.hpp"
#include "Utils/RessourceManager.hpp"
#include "Core/Components.hh"

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
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return (true);
}

void    RenderingSystem::update(EntityManager& em, float elapsedTime)
{

    // Enable depth test
    //glEnable(GL_DEPTH_TEST);

    // Specify depth function
    //glDepthFunc(GL_GREATER);

    // Specify depth buffer clear value
    //glClearDepth(0.0f);

    // Clear screen and depth buffer
    glClear (GL_COLOR_BUFFER_BIT);
    forEachEntity(em, [&](Entity *entity) {
        sPositionComponent *position = entity->getComponent<sPositionComponent>();
        RenderEntity &renderEntity = getRenderEntity(entity);

        // Orthogonal projection matrice
        glm::mat4 ortho = glm::ortho(0.0f, (float)GameWindow::getInstance()->getWidth(), 0.0f, (float)GameWindow::getInstance()->getHeight());
        GLint uniProj = _shaderProgram.getUniformLocation("proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(ortho));

        // Model matrice
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(position->x, position->y, 0.0f));
        GLint uniTrans = _shaderProgram.getUniformLocation("trans");
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        // Bind texture
        //renderEntity.texture->bind();

        // Bind Vertex array buffer
        renderEntity.buffer.bind();

        // Draw to screen
        glDrawElements(GL_TRIANGLES, renderEntity.buffer.getIndicesNb(), GL_UNSIGNED_INT, 0);
    });

    // Display screen
    GameWindow::getInstance()->display();
}

RenderEntity&   RenderingSystem::getRenderEntity(Entity* entity)
{
    int id = entity->id;

    // The entity does not exist in the render system
    if (_renderEntities.find(id) == _renderEntities.end())
    {
        _renderEntities[id] = new RenderEntity;

        // Initialize entity texture
  /*      sRenderComponent *sprite = entity->getComponent<sRenderComponent>();
        _renderEntities[id]->texture = &RessourceManager::getInstance()->getTexture(sprite->texture);*/

/*        float textureWidth = _renderEntities[id]->texture->getWidth();
        float textureHeight = _renderEntities[id]->texture->getHeight();*/

        // Init entity buffers
        Vertex vertices[] = {
            {glm::vec3(0.0f,  100.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Top Left (red)
            {glm::vec3(100.0f,  100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},  // Top Right (blue)
            {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Bottom Left (green)
            {glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}  // Bottom Right (red)
        };

        GLuint indices[] = {
            0, 1, 2,
            1, 2, 3
        };

        _renderEntities[id]->buffer.updateData(vertices, 4, indices, 6);
    }

    return *_renderEntities[id];
}