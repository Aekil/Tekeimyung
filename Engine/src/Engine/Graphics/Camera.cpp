/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Graphics/Camera.hpp>

Camera*     Camera::_instance = nullptr;

Camera::Camera(): _needUpdateView(true), _needUpdateProj(true), _fov(45.0f),
                    _near(0.1f), _far(1300.0f),
                    _projType(Camera::eProj::PERSPECTIVE)
{
    _constants.view = glm::mat4(1.0f);
    _ubo.setBindingPoint(1);
    _ubo.init(sizeof(_constants));

    _viewportRect.offset.x = 0.0f;
    _viewportRect.offset.y = 0.0f;
    _viewportRect.extent.width = 1.0f;
    _viewportRect.extent.height = 1.0f;
    _projSize = 500;
    updateViewport();
}

Camera::~Camera()
{
    // We don't want the Renderer camera singleton to use our camera adress
    // Because it will be deleted
    auto renderer = Renderer::_instance;
    if (renderer &&
        renderer->getCurrentCamera() == this)
    {
        renderer->setCurrentCamera(nullptr);
    }
}

Camera::Camera(const Camera& camera)
{
    _viewportRect = camera._viewportRect;
    _viewport = camera._viewport;
    _projSize = camera._projSize;
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _projType = camera._projType;
}

Camera& Camera::operator=(const Camera& camera)
{
    _viewportRect = camera._viewportRect;
    _viewport = camera._viewport;
    _projSize = camera._projSize;
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _projType = camera._projType;
    return (*this);
}

const glm::mat4&    Camera::getView()
{
    if (_needUpdateView)
    {
        updateView();
    }
    return (_view);
}

const glm::mat4&    Camera::getProj()
{
    if (_needUpdateProj)
    {
        updateProj();
    }
    return (_proj);
}

float   Camera::getAspect() const
{
    return (_viewport.extent.width / _viewport.extent.height);
}

float   Camera::getFov() const
{
    return (_fov);
}

float   Camera::getFar() const
{
    return (_far);
}

float   Camera::getNear() const
{
    return (_near);
}

Camera::eProj   Camera::getProjType() const
{
    return (_projType);
}

float   Camera::getProjSize() const
{
    return (_projSize);
}

const Camera::sViewport&  Camera::getViewportRect() const
{
    return _viewportRect;
}

const Camera::sViewport&  Camera::getViewport() const
{
    return _viewport;
}

void    Camera::setFov(float fov)
{
    _fov = fov;
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::setNear(float near)
{
    _near = near;
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::setFar(float far)
{
    _far = far;
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::setViewportRect(const sViewport& viewportRect)
{
    _viewportRect = viewportRect;
    updateViewport();
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::setProjType(eProj projType)
{
    _projType = projType;
    _needUpdateProj = _needUpdateView = true;
    isDirty(true);
}

void    Camera::setProjSize(float projSize)
{
    _projSize = projSize;
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::updateViewport()
{
    float windowBufferWidth = (float)GameWindow::getInstance()->getBufferWidth();
    float windowBufferHeight = (float)GameWindow::getInstance()->getBufferHeight();

    _viewport.offset.x = windowBufferWidth * _viewportRect.offset.x;
    _viewport.offset.y = windowBufferHeight * _viewportRect.offset.y;
    _viewport.extent.width = windowBufferWidth * _viewportRect.extent.width;
    _viewport.extent.height = windowBufferHeight * _viewportRect.extent.height;
    _needUpdateProj = true;
    isDirty(true);
}

void    Camera::updateUBO()
{
    if (isDirty())
    {
        // Camera view depend on translate, so force update view
        // as isDirty() could be true if translate is called
        _needUpdateView = true;

        _constants.proj = getProj();
        _constants.view = getView();
        _constants.pos = getPos();
        _constants.dir = getDirection();
        _ubo.update(&_constants, sizeof(_constants));
        isDirty(false);
    }
}

UniformBuffer&  Camera::getUBO()
{
    return (_ubo);
}

Ray     Camera::screenPosToRay(float posX, float posY)
{
    auto gameWindow = GameWindow::getInstance();

    glm::vec3 nearScreen(posX, gameWindow->getBufferHeight() - posY, 0.0f);
    glm::vec3 farScreen(posX, gameWindow->getBufferHeight() - posY, 1.0f);

    // Unproject 2D points to get 3D points
    // Get 3D point on near plane
    glm::vec4 viewport = glm::vec4(getViewport().offset.x,
                                getViewport().offset.y,
                                getViewport().extent.width,
                                getViewport().extent.height);
    glm::vec3 nearPoint = glm::unProject(nearScreen, getView(), getProj(), viewport);
    // Get 3D point on far plane
    glm::vec3 farPoint = glm::unProject(farScreen, getView(), getProj(), viewport);

    return Ray(nearPoint, farPoint - nearPoint);
}

void    Camera::setInstance(Camera* instance)
{
    _instance = instance;
}

Camera* Camera::getInstance()
{
    return (_instance);
}

void    Camera::updateProj()
{
    if (_projType == Camera::eProj::ORTHOGRAPHIC_3D)
    {
        glm::vec2 projSize;
        projSize.x = getProjSize() * getAspect();
        projSize.y = getProjSize();

        _proj = glm::ortho(projSize.x / 2.0f * -1.0f,
                            projSize.x / 2.0f,
                            projSize.y / 2.0f * -1.0f,
                            projSize.y / 2.0f,
                            _near, _far);
    }
    else if (_projType == Camera::eProj::ORTHOGRAPHIC_2D)
    {
        glm::vec2 projSize(_viewport.extent.width, _viewport.extent.height);
        _proj = glm::ortho(_viewport.offset.x, projSize.x + _viewport.offset.x,
                                        _viewport.offset.y, projSize.y + _viewport.offset.y,
                                        0.0f, _far);
    }
    else if (_projType == Camera::eProj::PERSPECTIVE)
    {
        _proj = glm::perspective(glm::radians(_fov), getAspect(), _near, _far);
    }
    else
        ASSERT(0, "Unknown projection type");

    _needUpdateProj = false;
}

void    Camera::updateView()
{
    glm::mat4 rotate = glm::mat4_cast(getOrientation());
    glm::mat4 translate = glm::translate(glm::mat4(1.0), -getPos());
    _view = rotate * translate;

    _needUpdateView = false;
}

glm::vec3   Camera::getVisibleAreaSize(const glm::vec3& projSize) const
{
    glm::vec3 visibleProjSize = projSize;
    glm::vec2 maxProjSize = {projSize.x / getViewportRect().extent.width,
                             projSize.y / getViewportRect().extent.height};
    glm::vec2 offset = {getViewportRect().offset.x * maxProjSize.x,
                        getViewportRect().offset.y * maxProjSize.y};

    // Update X size
    {
        if (offset.x >= 0)
        {
            float outScreen = maxProjSize.x - (projSize.x + offset.x);
            if (outScreen < 0.0f)
            {
                visibleProjSize.x += outScreen;
            }
        }
        else
        {
            visibleProjSize.x += offset.x;
        }
    }

    // Update Y size
    {
        if (offset.y >= 0)
        {
            float outScreen = maxProjSize.y - (projSize.y + offset.y);
            if (outScreen < 0.0f)
            {
                visibleProjSize.y += outScreen;
            }
        }
        else
        {
            visibleProjSize.y += offset.y;
        }
    }

    return (visibleProjSize);
}
