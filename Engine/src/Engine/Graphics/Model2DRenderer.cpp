/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Model2DRenderer.hpp>
#include <Engine/Graphics/Renderer.hpp>

std::shared_ptr<Model2DRenderer>   Model2DRenderer::_instance = nullptr;

std::shared_ptr<Model2DRenderer>    Model2DRenderer::getInstance()
{
    if (!_instance)
    {
        _instance = std::make_shared<Model2DRenderer>();
        _instance->initialize();
    }

    return (_instance);
}

void    Model2DRenderer::initialize()
{
    // Setup camera
    // We rotate the camera to have an isometric view
    _2DRenderCamera.rotate(-35.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    _2DRenderCamera.rotate(47.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    _2DRenderCamera.setNear(0.1f);
    _2DRenderCamera.setFar(5000.0f);
    _2DRenderCamera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);

    // Setup uniform buffer to pass model transformation and color
    _2DRenderBuffer.init(sizeof(glm::mat4) + sizeof(glm::vec4), GL_SHADER_STORAGE_BUFFER);
    _2DRenderBuffer.setBindingPoint(3);


    // Setup light
    _2DRenderLight.setAmbient({1.0f, 1.0f, 1.0f});
    _2DRenderLight.setDiffuse({1.0f, 1.0f, 1.0f});
}

std::unique_ptr<Texture>    Model2DRenderer::generateTextureFromModel(sRenderComponent* renderComponent, uint32_t width, uint32_t height)
{
    float windowBufferWidth = (float)GameWindow::getInstance()->getBufferWidth();
    float windowBufferHeight = (float)GameWindow::getInstance()->getBufferHeight();

    ModelInstance* model = renderComponent->getModelInstance();
    auto renderer = Renderer::getInstance();

    // Init framebuffer
    if (!renderer->setupFramebuffers(_2DFramebuffer, _2DBlurFramebuffers, width, height))
    {
        LOG_INFO("Failed to setup 2D render frame buffers");
        return (nullptr);
    }

    // Set camera viewport
    {
        Camera::sViewport viewportRect;
        viewportRect.offset.x = 0.0f;
        viewportRect.offset.y = 0.0f;
        viewportRect.extent.width = (float)width / windowBufferWidth;
        viewportRect.extent.height = (float)height / windowBufferHeight;
        _2DRenderCamera.setViewportRect(viewportRect);
    }


    // Calculate camera projection size so the model fit all the texture
    // The size is the distance between the front left bottom and back right top corners of the model
    // We don't take the real position of the corners, but create a virtual 3D box with the size of the model
    {
        glm::vec3 frontLeftBottom = glm::vec3(_2DRenderCamera.getView() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glm::vec3 backRightTop = glm::vec3(_2DRenderCamera.getView() * glm::vec4(model->getModel()->getSize().x, model->getModel()->getSize().y, -model->getModel()->getSize().z, 1.0f));
        float projSize = glm::distance(frontLeftBottom, backRightTop);
        // Inverse the projection so the texture otherwise the texture will be reversed
        _2DRenderCamera.setProjSize(-projSize);
    }

    // Place the object in front of the camera and center it
    {
        glm::vec3 cameraDir = glm::normalize(_2DRenderCamera.getDirection());

        // Calculate the translation the model need to have his pivot point centered
        glm::vec3 modelPivot = glm::vec3(0.0f, -(model->getModel()->getSize().y / 2.0f + model->getModel()->getMin().y), 0.0f);
        modelPivot += glm::vec3(-(model->getModel()->getSize().x / 2.0f + model->getModel()->getMin().x), 0.0f, 0.0f);
        modelPivot += glm::vec3(0.0f, 0.0f, -(model->getModel()->getSize().z / 2.0f + model->getModel()->getMin().z));

        // We place the object at a distance of 200 in front of the camera and center it
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), cameraDir * -200.0f + modelPivot);

        // Update uniform buffer
        _2DRenderBuffer.update((void*)glm::value_ptr(transform), sizeof(glm::mat4));
        _2DRenderBuffer.update((void*)glm::value_ptr(renderComponent->color), sizeof(glm::vec4), sizeof(glm::mat4));
    }

    // The light have the same direction as the camera
    _2DRenderLight.setDirection(-glm::normalize(_2DRenderCamera.getDirection()));

    // Update render queue
    _2DRenderQueue.clear();
    _2DRenderQueue.addModel(model, &_2DRenderBuffer, 0, _2DRenderBuffer.getSize());
    _2DRenderQueue.addLight(&_2DRenderLight);

    // Render the model in our frame buffer
    _2DFramebuffer.use(GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->sceneRenderPass(&_2DRenderCamera, _2DRenderQueue);

    // Bloom pass
    {
        renderer->bloomPass(_2DFramebuffer.getColorAttachments()[1].get(), _2DBlurFramebuffers);
        glViewport(0,
                    0,
                    (uint32_t)width,
                    (uint32_t)height);

        renderer->_finalBlendingShaderProgram.use();
        glUniform1i(renderer->_finalBlendingShaderProgram.getUniformLocation("image"), 0);

        _2DFramebuffer.use(GL_FRAMEBUFFER);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        for (uint32_t i = 0; i < _2DBlurFramebuffers.size(); ++i)
        {
            _2DBlurFramebuffers[i][1].getColorAttachments()[0]->bind();
            renderer->_screenPlane.bind();

            glDrawElements(GL_TRIANGLES,
                        6,
                        GL_UNSIGNED_INT,
                        0);
        }
        _2DFramebuffer.getColorAttachments()[0]->bind();

        glDrawElements(GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    0);

        glDisable(GL_BLEND);
    }

    _2DFramebuffer.unBind(GL_FRAMEBUFFER);

    auto texture = std::move(_2DFramebuffer.getColorAttachments()[0]);

    // Clear the framebuffer because we need to return the first color attachment
    _2DFramebuffer.removeColorAttachments();

    return (texture);
}

std::unique_ptr<Texture>    Model2DRenderer::generateTextureFromModel(const std::string& entityName,
                                                                        uint32_t width,
                                                                        uint32_t height)
{
    auto em = EntityFactory::getBindedEntityManager();
    Entity* entity = EntityFactory::createEntity(entityName);

    sRenderComponent* entityRender = entity->getComponent<sRenderComponent>();
    if (!entityRender)
    {
        LOG_ERROR("Model2DRenderer::generateTextureFromModel : the entity has no sRenderComponent", entityName.c_str());
        return (nullptr);
    }

    std::unique_ptr<Texture> texture = Model2DRenderer::getInstance()->generateTextureFromModel(entityRender, width, height);

    // Destroy the entity, we don't need it anymore
    em->destroyEntityRegister(entity);

    return (texture);
}

void    Model2DRenderer::renderModelOnPlane(const std::string& modelEntityName,
                                                                const std::string& planeEntityName,
                                                                uint32_t width,
                                                                uint32_t height)
{
    auto em = EntityFactory::getBindedEntityManager();
    Entity* planeEntity = em->getEntityByTag(planeEntityName);
    if (!planeEntity)
    {
        LOG_ERROR("Model2DRenderer::renderModelOnPlane : Plane entity not found in current EntityManager");
        return;
    }

    sRenderComponent* planeEntityRender = planeEntity->getComponent<sRenderComponent>();
    if (!planeEntityRender)
    {
        LOG_ERROR("Model2DRenderer::renderModelOnPlane : Can't get sRenderComponent on plane entity %s", planeEntityName.c_str());
        return;
    }

    ModelInstance* planeEntityModelInstance = planeEntityRender->getModelInstance();
    if (!planeEntityModelInstance->getMeshsInstances().size())
    {
        LOG_ERROR("Model2DRenderer::renderModelOnPlane : The plane entity %s has no mesh", planeEntityName.c_str());
        return;
    }

    std::unique_ptr<Texture> texture = Model2DRenderer::getInstance()->generateTextureFromModel(modelEntityName, width, height);
    // Let Model2DRenderer own the texture
    _renderedTextures.push_back(std::move(texture));

    planeEntityModelInstance->getMeshsInstances()[0]->getMaterial()->setTexture(Texture::eType::DIFFUSE, _renderedTextures.back().get());
    planeEntityModelInstance->getMeshsInstances()[0]->getMaterial()->setTexture(Texture::eType::AMBIENT, _renderedTextures.back().get());
}
