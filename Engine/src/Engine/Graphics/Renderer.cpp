/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Graphics/UI/Font.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Graphics/Renderer.hpp>

std::shared_ptr<Renderer>   Renderer::_instance = nullptr;

Renderer::Renderer(): _currentCamera(nullptr)
{
    _defaultLight.setDirection({-1.0f, -0.3f, 0.3f});

    _UILight.setDirection({0.0f, 0.0f, 1.0f});
    _UILight.setAmbient({1.0f, 1.0f, 1.0f});
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
        std::vector<std::vector<Material::eOption> > permutations = {
            { },
            { Material::eOption::TEXTURE_AMBIENT },
            { Material::eOption::TEXTURE_DIFFUSE },
            { Material::eOption::FACE_CAMERA },

            { Material::eOption::TEXTURE_AMBIENT,
                Material::eOption::TEXTURE_DIFFUSE },
            { Material::eOption::TEXTURE_AMBIENT,
                Material::eOption::FACE_CAMERA },
            { Material::eOption::TEXTURE_DIFFUSE,
                Material::eOption::FACE_CAMERA },

            { Material::eOption::TEXTURE_AMBIENT,
                Material::eOption::TEXTURE_DIFFUSE,
                Material::eOption::FACE_CAMERA }
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
        initTextRendering();
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    onWindowResize();

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
}

void    Renderer::render(Camera* camera, RenderQueue& renderQueue)
{


    // Render scene objects
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

            // Set viewport
            glViewport((uint32_t)camera->getViewport().offset.x,
                        (uint32_t)camera->getViewport().offset.y,
                        (uint32_t)camera->getViewport().extent.width,
                        (uint32_t)camera->getViewport().extent.height);

            camera->updateUBO();
            camera->getUBO().bind();

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

    // Render UI objects
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

    // Render texts
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        _textShaderProgram.use();
        glUniform1i(_textShaderProgram.getUniformLocation("textImage"), 0);
        renderTexts(renderQueue.getTexts(), renderQueue.getTextsNb());
        _currentShaderProgram = nullptr;
    }

    // Enable depth buffer write and depth test for non-UI objects
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // Disable blending for opaque objects
    glDisable(GL_BLEND);

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

void    Renderer::renderTexts(std::vector<sRenderableText>& texts,
                                uint32_t textsNb)
{
    if (textsNb == 0)
        return;

    _UILight.bind();

    for (uint32_t i = 0; i < textsNb; ++i)
    {
        auto& renderText = texts[i];
        auto& text = renderText.text;

        const Font* font = text.getFont();
        float fontScale = (float)text.getFontSize() / 200.0f;
        ASSERT(font != nullptr, "Text should have font")
        glm::vec2 pos = renderText.pos;
        pos.y -= text.getFontSize();
        _textUBO.update((void*)&text.getColor(), sizeof(glm::vec4), sizeof(glm::mat4));

        for (uint32_t j = 0; j < text.getContent().size(); ++j)
        {
            char c = text.getContent()[j];
            if (c == '\n')
            {
                pos.x = renderText.pos.x;
                pos.y -= text.getFontSize();
                continue;
            }

            auto char_ = font->getChar(c);
            if (!char_)
                continue;

            glm::mat4 translate = glm::translate(glm::mat4(1.0f), {
                pos.x + (char_->bearing.x * fontScale),
                pos.y + (char_->bearing.y) * fontScale,
                0.0f
            });
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), {
                char_->texture.getWidth() * fontScale,
                char_->texture.getHeight() * fontScale,
                1.0f
            });

            //glm::mat4 transform = glm::mat4(1.0f);
            glm::mat4 transform = translate * scale;
            _textUBO.update(&transform, sizeof(glm::mat4), 0);
            _textUBO.bind();
            _textPlane.bind();
            char_->texture.bind();

            glDrawElements(GL_TRIANGLES,
                            6,
                            GL_UNSIGNED_INT,
                            0);
            pos.x += (char_->advance >> 6) * fontScale;
        }

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

void    Renderer::initTextRendering()
{
    // Set alignment to 1 because fonts textures pixels only used 1 byte
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    _textShaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert", {});
    _textShaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader-text.frag", {});
    _textShaderProgram.link();

    // Init text plane buffer
    {
       Vertex vertexs[] {
            //1. pos
            //2. color
            //3. normal
            //4. texture uv
            {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},  // Top Left
            {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},  // Top Right
            {glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Bottom Left
            {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}  // Bottom Right
        };

        GLuint indices[] = {
            0, 2, 3,
            3, 1, 0
        };

        _textPlane.updateData(vertexs, 4, indices, 6);
    }

    _textUBO.init((uint32_t)(sizeof(glm::mat4) + sizeof(glm::vec4)),
                    GL_SHADER_STORAGE_BUFFER);
    _textUBO.setBindingPoint(3);
}
