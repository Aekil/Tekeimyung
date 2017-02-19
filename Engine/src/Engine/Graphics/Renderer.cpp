/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Graphics/Renderer.hpp>

std::shared_ptr<Renderer>   Renderer::_instance = nullptr;

Renderer::Renderer(): _currentCamera(nullptr) {}

Renderer::~Renderer() {}

std::shared_ptr<Renderer>   Renderer::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<Renderer>();

    return (_instance);
}

bool    Renderer::initialize()
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
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    onWindowResize();

    // Enable blend for transparency
/*    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Activate back culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    return (true);
}

void    Renderer::onWindowResize()
{
    auto gameWindow = GameWindow::getInstance();

    // Set camera screen
    Camera::sScreen screen;
    screen.right = (float)gameWindow->getBufferWidth();
    screen.left = 0;
    screen.top = (float)gameWindow->getBufferHeight();
    screen.bottom = 0;
    _UICamera.setScreen(screen);
    _UICamera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
    _UICamera.updateUBO();
}

void    Renderer::render(Camera* camera, RenderQueue& renderQueue)
{
    _currentCamera = camera;

    // Scene objects
    {
        camera->updateUBO();
        camera->getUBO().bind(_shaderProgram, "camera");

        renderOpaqueObjects(camera, renderQueue.getOpaqueMeshs(), renderQueue.getOpaqueMeshsNb());

        // Enable blend to blend transparent ojects and particles
        glEnable(GL_BLEND);
        // Disable write to the depth buffer so that the depth of transparent objects is not written
        // because we don't want a transparent object to hide an other transparent object
        glDepthMask(GL_FALSE);
        renderTransparentObjects(camera, renderQueue.getTransparentMeshs(), renderQueue.getTransparentMeshsNb());

        // Enable depth buffer for opaque objects
        //glDepthMask(GL_TRUE);
        // Disable blending for opaque objects
        glDisable(GL_BLEND);
    }

    // UI objects
    {
        _UICamera.getUBO().bind(_shaderProgram, "camera");

        renderOpaqueObjects(camera, renderQueue.getUIOpaqueMeshs(), renderQueue.getUIOpaqueMeshsNb());

        // Enable blend to blend transparent ojects and particles
        glEnable(GL_BLEND);
        // Disable write to the depth buffer so that the depth of transparent objects is not written
        // because we don't want a transparent object to hide an other transparent object
        glDepthMask(GL_FALSE);
        renderTransparentObjects(camera, renderQueue.getUITransparentMeshs(), renderQueue.getUITransparentMeshsNb());

        // Enable depth buffer for opaque objects
        glDepthMask(GL_TRUE);
        // Disable blending for opaque objects
        glDisable(GL_BLEND);
    }

}

void    Renderer::renderOpaqueObjects(Camera* camera, std::vector<sRenderableMesh>& meshs, uint32_t meshsNb)
{
    for (uint32_t i = 0; i < meshsNb; ++i)
    {
        auto& renderableMesh = meshs[i];
        Mesh* mesh = renderableMesh.meshInstance->getMesh();
        // Model matrix
        static GLint uniModel = _shaderProgram.getUniformLocation("model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(renderableMesh.transform));

        // Model color
        static GLint colorModel = _shaderProgram.getUniformLocation("modelColor");
        glUniform4f(colorModel, renderableMesh.color.x, renderableMesh.color.y, renderableMesh.color.z, renderableMesh.color.w);

        // Bind buffer
        mesh->getModel()->getBuffer().bind();

        renderableMesh.meshInstance->getMaterial()->bind(_shaderProgram);

        // Draw to screen
        glDrawElements(mesh->getModel()->getPrimitiveType(), (GLuint)mesh->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
    }
}

bool sortTransparent(const sRenderableMesh& lhs, const sRenderableMesh& rhs)
{
    Material* lhsMaterial = lhs.meshInstance->getMaterial();
    Material* rhsMaterial = rhs.meshInstance->getMaterial();

    // Sort by src blend and by dst blend
    return (lhsMaterial->_srcBlend < rhsMaterial->_srcBlend ||
        lhsMaterial->_srcBlend == rhsMaterial->_srcBlend && lhsMaterial->_dstBlend < rhsMaterial->_dstBlend);
}

void    Renderer::renderTransparentObjects(Camera* camera, std::vector<sRenderableMesh>& meshs, uint32_t meshsNb)
{
    if (meshsNb == 0)
        return;

    std::sort(meshs.begin(), meshs.begin() + meshsNb, sortTransparent);

    GLenum lastSrcBlend = meshs[0].meshInstance->getMaterial()->_srcBlend;
    GLenum lastDstBlend = meshs[0].meshInstance->getMaterial()->_dstBlend;
    glBlendFunc(lastSrcBlend, lastDstBlend);

    for (uint32_t i = 0; i < meshsNb; ++i)
    {
        auto& renderableMesh = meshs[i];
        Mesh* mesh = renderableMesh.meshInstance->getMesh();
        Material* material = renderableMesh.meshInstance->getMaterial();

        // Change blend mode
        if (lastSrcBlend != material->_srcBlend ||
            lastDstBlend != material->_dstBlend)
        {
            glBlendFunc(material->_srcBlend, material->_dstBlend);
            lastSrcBlend = material->_srcBlend;
            lastDstBlend = material->_dstBlend;
        }

        // Model matrix
        static GLint uniModel = _shaderProgram.getUniformLocation("model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(renderableMesh.transform));

        // Model color
        static GLint colorModel = _shaderProgram.getUniformLocation("modelColor");
        glUniform4f(colorModel, renderableMesh.color.x, renderableMesh.color.y, renderableMesh.color.z, renderableMesh.color.w);

        // Bind buffer
        mesh->getModel()->getBuffer().bind();

        material->bind(_shaderProgram);

        // Draw to screen
        glDrawElements(mesh->getModel()->getPrimitiveType(), (GLuint)mesh->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
    }
}

ShaderProgram&  Renderer::getShaderProgram()
{
    return (_shaderProgram);
}

Camera* Renderer::getCurrentCamera()
{
    return (_currentCamera);
}

void    Renderer::setCurrentCamera(Camera* camera)
{
    _currentCamera = camera;
}
