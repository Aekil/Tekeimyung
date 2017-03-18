/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <ImGuizmo.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Graphics/Material.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Graphics/Renderer.hpp>

std::shared_ptr<Renderer>   Renderer::_instance = nullptr;

Renderer::Renderer(): _currentCamera(nullptr)
{
    _defaultLight.setDirection({-1.0f, -0.3f, 0.3f});

    _UILight.setDirection({0.0f, 0.0f, 1.0f});
    _UILight.setAmbient({0.8f, 0.8f, 0.8f});
    _UILight.setDiffuse({0.0f, 0.0f, 0.0f});

    _UICamera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
    _UICamera.updateViewport();
    _UICamera.updateUBO();
}

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
        setupShaderPrograms();
        onWindowResize();
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }


    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Activate back culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Enable scissor
    glEnable(GL_SCISSOR_TEST);
    return (true);
}

void    Renderer::onWindowResize()
{
    _UICamera.updateViewport();
    _UICamera.updateUBO();

    if (!setupFrameBuffer())
    {
        EXCEPT(InternalErrorException, "Failed to setup frame buffer");
    }
}

Camera* Renderer::getCurrentCamera()
{
    return (_currentCamera);
}

void    Renderer::setCurrentCamera(Camera* camera)
{
    _currentCamera = camera;
}

void    Renderer::beginFrame()
{
    // Clear window screen
    glClear(GL_COLOR_BUFFER_BIT);

    _frameBuffer.use(GL_FRAMEBUFFER);

    // Clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _frameBuffer.unBind(GL_FRAMEBUFFER);

    for (auto& framebuffer: _blurFrameBuffers)
    {
        framebuffer.use(GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT);
        framebuffer.unBind(GL_FRAMEBUFFER);
    }

    ImGui_ImplGlfwGL3_NewFrame();
    ImGuizmo::BeginFrame();
}

void    Renderer::endFrame()
{
    // Apply bloom and then blend the scene with bloom texture
    {
        glDisable(GL_DEPTH_TEST);
        bloomPass();
        finalBlendingPass();
        glEnable(GL_DEPTH_TEST);
        _currentShaderProgram = nullptr;
    }

    // Display imgui windows
    ImGui::Render();

    // Display screen
    GameWindow::getInstance()->display();
}

void    Renderer::render(Camera* camera, RenderQueue& renderQueue)
{
    // All the renders will use the color attachments and the depth buffer of the framebuffer
    _frameBuffer.use(GL_FRAMEBUFFER);
    sceneRenderPass(camera, renderQueue);
    _frameBuffer.unBind(GL_FRAMEBUFFER);
}

void    Renderer::sceneRenderPass(Camera* camera, RenderQueue& renderQueue)
{
    // Scene objects
    {
        if (camera)
        {
            _currentCamera = camera;
            auto& lights = renderQueue.getLights();
            uint32_t lightsNb = renderQueue.getLightsNb();

            // Set default light
            if (lightsNb == 0)
            {
                lights[0] = &_defaultLight;
                lightsNb = 1;
            }

            camera->updateViewport();
            camera->updateUBO();
            camera->getUBO().bind();

            glViewport((uint32_t)camera->getViewport().offset.x,
                        (uint32_t)camera->getViewport().offset.y,
                        (uint32_t)camera->getViewport().extent.width,
                        (uint32_t)camera->getViewport().extent.height);

            renderOpaqueObjects(renderQueue.getOpaqueMeshs(), renderQueue.getOpaqueMeshsNb(), lights, lightsNb);

            // Enable blend to blend transparent ojects and particles
            glEnable(GL_BLEND);
            // Disable write to the depth buffer so that the depth of transparent objects is not written
            // because we don't want a transparent object to hide an other transparent object
            glDepthMask(GL_FALSE);
            renderTransparentObjects(renderQueue.getTransparentMeshs(), renderQueue.getTransparentMeshsNb(), lights, lightsNb);
        }
        else if (renderQueue.getOpaqueMeshsNb() + renderQueue.getTransparentMeshsNb() != 0)
        {
            LOG_WARN("Attempt to render non-UI objects with no camera");
        }
    }

    // Disable write to the depth buffer and depth test because UI is sort with layers
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    // Disable blending for opaque objects
    glDisable(GL_BLEND);

    // UI objects
    {
        // Use this light for UI
        std::vector<Light*> lights = {&_UILight};

        // Set viewport
        glViewport((uint32_t)_UICamera.getViewport().offset.x,
                    (uint32_t)_UICamera.getViewport().offset.y,
                    (uint32_t)_UICamera.getViewport().extent.width,
                    (uint32_t)_UICamera.getViewport().extent.height);
        _UICamera.getUBO().bind();


        renderOpaqueObjects(renderQueue.getUIOpaqueMeshs(), renderQueue.getUIOpaqueMeshsNb(), lights, 1);

        // Enable blend to blend transparent ojects and particles
        glEnable(GL_BLEND);
        renderTransparentObjects(renderQueue.getUITransparentMeshs(), renderQueue.getUITransparentMeshsNb(), lights, 1);

    }

    // Enable depth buffer write and depth test for non-UI objects
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // Disable blending for opaque objects
    glDisable(GL_BLEND);
}

void    Renderer::bloomPass()
{
    GLboolean horizontal = true;
    glViewport(0,
                0,
                (uint32_t)_blurColorAttachments[0]->getWidth(),
                (uint32_t)_blurColorAttachments[0]->getHeight());

    _blurShaderProgram.use();
    glUniform1i(_blurShaderProgram.getUniformLocation("image"), 0);

    bool firstPass = true;

    for (uint32_t i = 0; i < 10; ++i)
    {
        _blurFrameBuffers[horizontal].use(GL_FRAMEBUFFER);
        glUniform1i(_blurShaderProgram.getUniformLocation("horizontal"), horizontal);

        // Render the bright scene into the framebuffer for the first pass
        // For the other passes, we alternate between the 2 framebuffers of the ping-pong framebuffers
        if (firstPass)
        {
            firstPass = false;
            _sceneBrightColorAttachment->bind();
        }
        else
        {
            _blurColorAttachments[!horizontal]->bind();
        }

        _screenPlane.bind();

        glDrawElements(GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    0);

        horizontal = !horizontal;
    }

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _lastBlurColorAttachment = _blurColorAttachments[!horizontal].get();
}

void    Renderer::finalBlendingPass()
{
    GLsizei windowBufferWidth = (GLsizei)GameWindow::getInstance()->getBufferWidth();
    GLsizei windowBufferHeight = (GLsizei)GameWindow::getInstance()->getBufferHeight();

    glViewport(0,
                0,
                (uint32_t)windowBufferWidth,
                (uint32_t)windowBufferHeight);

    _finalBlendingShaderProgram.use();
    glUniform1i(_finalBlendingShaderProgram.getUniformLocation("sceneTexture"), 0);
    glUniform1i(_finalBlendingShaderProgram.getUniformLocation("sceneBloomTexture"), 1);

    _sceneColorAttachment->bind();
    // Attached the last rendered blur color attachment
    _lastBlurColorAttachment->bind(GL_TEXTURE1);
    _screenPlane.bind();


    glDrawElements(GL_TRIANGLES,
                6,
                GL_UNSIGNED_INT,
                0);
}

bool sortOpaque(const sRenderableMesh& lhs, const sRenderableMesh& rhs)
{
    Material* lhsMaterial = lhs.meshInstance->getMaterial();
    Material* rhsMaterial = rhs.meshInstance->getMaterial();

    // Sort by layer and shaderProgram
    return (lhs.layer > rhs.layer ||
            lhs.layer > rhs.layer && lhsMaterial->getOptions() > rhsMaterial->getOptions());
}

void    Renderer::renderOpaqueObjects(std::vector<sRenderableMesh>& meshs,
                                    uint32_t meshsNb,
                                    std::vector<Light*>& lights,
                                    uint32_t lightsNb)
{
    if (meshsNb == 0)
        return;

    std::sort(meshs.begin(), meshs.begin() + meshsNb, sortOpaque);

    for (uint32_t j = 0; j < lightsNb; ++j)
    {
        for (uint32_t i = 0; i < meshsNb; ++i)
        {
            auto& renderableMesh = meshs[i];
            Mesh* mesh = renderableMesh.meshInstance->getMesh();
            Material* material = renderableMesh.meshInstance->getMaterial();

            // Bind new shader
            if (!_currentShaderProgram || _currentShaderProgram->getOptions() != material->getOptions())
            {
                _currentShaderProgram = &_shaderPrograms.at(material->getOptions());
                _currentShaderProgram->use();

                // Set texture location unit
                // Must be the same unit as material textures. See Material::loadFromAssimp
                glUniform1i(_currentShaderProgram->getUniformLocation("AmbientTexture"), 0);
                glUniform1i(_currentShaderProgram->getUniformLocation("DiffuseTexture"), 1);
                glUniform1i(_currentShaderProgram->getUniformLocation("BloomTexture"), 2);
                glUniform1i(_currentShaderProgram->getUniformLocation("BloomTextureAlpha"), 3);
            }

            // Bind buffer
            mesh->getModel()->getBuffer().bind();

            material->bind();
            lights[j]->bind();
            renderableMesh.ubo->bind(renderableMesh.uboOffset, renderableMesh.uboSize);

            GLuint primitive = mesh->getModel()->getPrimitiveType();
            if (material->wireframe)
                primitive = GL_LINE_STRIP;

            // Draw to screen
            if (renderableMesh.instancesNb > 0)
            {
                glDrawElementsInstanced(primitive,
                                (GLuint)mesh->indices.size(),
                                GL_UNSIGNED_INT,
                                BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)),
                                renderableMesh.instancesNb);
            }
            else
            {
                glDrawElements(primitive,
                                (GLuint)mesh->indices.size(),
                                GL_UNSIGNED_INT,
                                BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
            }
        }
    }
}

bool sortTransparent(const sRenderableMesh& lhs, const sRenderableMesh& rhs)
{
    Material* lhsMaterial = lhs.meshInstance->getMaterial();
    Material* rhsMaterial = rhs.meshInstance->getMaterial();

    // Sort by layer, shaderProgram, src blend and dst blend
    return (lhs.layer > rhs.layer ||
        lhs.layer > rhs.layer && lhsMaterial->getOptions() > rhsMaterial->getOptions() ||
        lhs.layer > rhs.layer && lhsMaterial->getOptions() == rhsMaterial->getOptions() && lhsMaterial->srcBlend < rhsMaterial->srcBlend ||
        lhs.layer > rhs.layer && lhsMaterial->getOptions() > rhsMaterial->getOptions() && lhsMaterial->srcBlend == rhsMaterial->srcBlend && lhsMaterial->dstBlend < rhsMaterial->dstBlend);
}

void    Renderer::renderTransparentObjects(std::vector<sRenderableMesh>& meshs,
                                            uint32_t meshsNb,
                                            std::vector<Light*>& lights,
                                            uint32_t lightsNb)
{
    if (meshsNb == 0)
        return;

    std::sort(meshs.begin(), meshs.begin() + meshsNb, sortTransparent);

    GLenum lastSrcBlend = meshs[0].meshInstance->getMaterial()->srcBlend;
    GLenum lastDstBlend = meshs[0].meshInstance->getMaterial()->dstBlend;
    glBlendFunc(lastSrcBlend, lastDstBlend);

    for (uint32_t j = 0; j < lightsNb; ++j)
    {
        for (uint32_t i = 0; i < meshsNb; ++i)
        {
            auto& renderableMesh = meshs[i];
            Mesh* mesh = renderableMesh.meshInstance->getMesh();
            Material* material = renderableMesh.meshInstance->getMaterial();

            // Bind new shader
            if (!_currentShaderProgram || _currentShaderProgram->getOptions() != material->getOptions())
            {
                _currentShaderProgram = &_shaderPrograms.at(material->getOptions());
                _currentShaderProgram->use();

                // Set texture location unit
                // Must be the same unit as material textures. See Material::loadFromAssimp
                glUniform1i(_currentShaderProgram->getUniformLocation("AmbientTexture"), 0);
                glUniform1i(_currentShaderProgram->getUniformLocation("DiffuseTexture"), 1);
                glUniform1i(_currentShaderProgram->getUniformLocation("BloomTexture"), 2);
                glUniform1i(_currentShaderProgram->getUniformLocation("BloomTextureAlpha"), 3);
            }

            // Change blend mode
            if (lastSrcBlend != material->srcBlend ||
                lastDstBlend != material->dstBlend)
            {
                glBlendFunc(material->srcBlend, material->dstBlend);
                lastSrcBlend = material->srcBlend;
                lastDstBlend = material->dstBlend;
            }

            // Bind buffer
            mesh->getModel()->getBuffer().bind();

            material->bind();
            lights[j]->bind();
            renderableMesh.ubo->bind(renderableMesh.uboOffset, renderableMesh.uboSize);

            GLuint primitive = mesh->getModel()->getPrimitiveType();
            if (material->wireframe)
                primitive = GL_LINE_STRIP;

            // Draw to screen
            if (renderableMesh.instancesNb > 0)
            {
                glDrawElementsInstanced(primitive,
                                (GLuint)mesh->indices.size(),
                                GL_UNSIGNED_INT,
                                BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)),
                                renderableMesh.instancesNb);
            }
            else
            {
                glDrawElements(primitive,
                                (GLuint)mesh->indices.size(),
                                GL_UNSIGNED_INT,
                                BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
            }
        }
    }
}

bool    Renderer::setupFrameBuffer()
{
    GLsizei windowBufferWidth = (GLsizei)GameWindow::getInstance()->getBufferWidth();
    GLsizei windowBufferHeight = (GLsizei)GameWindow::getInstance()->getBufferHeight();
    bool complete = false;


    // Setup scene framebuffer
    {
        _sceneColorAttachment = Texture::create(windowBufferWidth,
                                                windowBufferHeight,
                                                GL_RGBA16F,
                                                GL_RGBA,
                                                GL_FLOAT);
        _sceneBrightColorAttachment = Texture::create(windowBufferWidth,
                                                windowBufferHeight,
                                                GL_RGBA16F,
                                                GL_RGBA,
                                                GL_FLOAT);
        _depthAttachment = Texture::create(windowBufferWidth,
                                            windowBufferHeight,
                                            GL_DEPTH24_STENCIL8,
                                            GL_DEPTH_STENCIL,
                                            GL_UNSIGNED_INT_24_8);

        _frameBuffer.bind(GL_FRAMEBUFFER);
        _frameBuffer.removeColorAttachments();
        _frameBuffer.addColorAttachment(*_sceneColorAttachment);
        _frameBuffer.addColorAttachment(*_sceneBrightColorAttachment);
        _frameBuffer.setDepthAttachment(*_depthAttachment);

        complete = _frameBuffer.isComplete();
        _frameBuffer.unBind(GL_FRAMEBUFFER);
    }

    // Setup ping-pong framebuffers
    // With ping-pong rendering, we render the scene in the first framebuffer,
    // then we blur the first framebuffer into the second one, then we blur the second one into the first, and so on
    {
        _blurColorAttachments[0] = Texture::create(static_cast<GLsizei>(windowBufferWidth),
                                                    static_cast<GLsizei>(windowBufferHeight),
                                                    GL_RGBA16F,
                                                    GL_RGBA,
                                                    GL_FLOAT);
        _blurColorAttachments[1] = Texture::create(static_cast<GLsizei>(windowBufferWidth),
                                                    static_cast<GLsizei>(windowBufferHeight),
                                                    GL_RGBA16F,
                                                    GL_RGBA,
                                                    GL_FLOAT);

        // First frame buffer
        _blurFrameBuffers[0].bind(GL_FRAMEBUFFER);
        _blurFrameBuffers[0].removeColorAttachments();
        _blurFrameBuffers[0].addColorAttachment(*_blurColorAttachments[0]);

        complete = complete && _blurFrameBuffers[0].isComplete();
        _blurFrameBuffers[0].unBind(GL_FRAMEBUFFER);

        // Second frame buffer
        _blurFrameBuffers[1].bind(GL_FRAMEBUFFER);
        _blurFrameBuffers[1].removeColorAttachments();
        _blurFrameBuffers[1].addColorAttachment(*_blurColorAttachments[1]);

        complete = complete && _blurFrameBuffers[1].isComplete();
        _blurFrameBuffers[1].unBind(GL_FRAMEBUFFER);
    }

    return (complete);
}

void    Renderer::setupShaderPrograms()
{
    // Init scene render programs
    // We create shader permutations for each material option
    // It will activate/deactivate preprocessor conditions in the shaders
    {
        std::vector<std::vector<Material::eOption> > permutations = {
            {  },
            { Material::eOption::TEXTURE_AMBIENT },
            { Material::eOption::TEXTURE_DIFFUSE },
            { Material::eOption::FACE_CAMERA },
            { Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::FACE_CAMERA },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::FACE_CAMERA, Material::eOption::BLOOM },
            { Material::eOption::FACE_CAMERA, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::FACE_CAMERA, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::FACE_CAMERA, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM },
            { Material::eOption::TEXTURE_AMBIENT, Material::eOption::TEXTURE_DIFFUSE, Material::eOption::FACE_CAMERA, Material::eOption::BLOOM, Material::eOption::TEXTURE_BLOOM }

        };


        for (const auto& options: permutations)
        {
            int optionFlag = 0;
            for (auto option: options)
            {
                optionFlag |= option;
            }

            auto& shaderProgram = _shaderPrograms[optionFlag];
            shaderProgram.setOptions(optionFlag);
            shaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert", options);
            shaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag", options);
            shaderProgram.link();
        }
    }

    // Init shader program of blur
    {
        _blurShaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader-single-plane.vert");
        _blurShaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader-blur.frag");
        _blurShaderProgram.link();
    }

    // Init shader program of final blending
    {
        _finalBlendingShaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader-single-plane.vert");
        _finalBlendingShaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader-final-blending.frag");
        _finalBlendingShaderProgram.link();
    }

    // Init buffer containing the plane vertices used for final blending
    {
       Vertex vertexs[] {
            //1. pos
            //2. color
            //3. normal
            //4. texture uv
            {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Top Left
            {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},  // Top Right
            {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},  // Bottom Left
            {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}  // Bottom Right
        };

        GLuint indices[] = {
            0, 2, 3,
            3, 1, 0
        };

        _screenPlane.updateData(vertexs, 4, indices, 6);
    }
}
