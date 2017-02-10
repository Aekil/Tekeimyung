/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Engine/Graphics/Camera.hpp>

Camera*     Camera::_instance = nullptr;

Camera::Camera(): _needUpdateView(true), _needUpdateProj(true), _fov(45.0f),
                    _aspect(1920.0f / 1080.0f), _near(0.1f), _far(1300.0f),
                    _up({0.0f, 1.0f, 0.0f}), _zoom(1.0f), _projType(Camera::eProj::ORTHOGRAPHIC_3D),
                    _needUpdateUbo(true)
{
    _constants.freezeRotations = 0;
    _constants.view = glm::mat4(1.0f);
    _windowBufferSize.x = (float)GameWindow::getInstance()->getBufferWidth();
    _windowBufferSize.y = (float)GameWindow::getInstance()->getBufferHeight();
}

Camera::~Camera() {}

bool    Camera::needUpdate() const
{
    return (_needUpdateProj || _needUpdateView);
}

const glm::vec3&    Camera::getPos() const
{
    return (_constants.pos);
}

const glm::mat4&    Camera::getView() const
{
    return (_constants.view);
}

const glm::mat4&    Camera::getProj() const
{
    return (_constants.proj);
}

float   Camera::getAspect() const
{
    return (_aspect);
}

float   Camera::getFov() const
{
    return (_fov);
}

Camera::eProj   Camera::getProjType() const
{
    return (_projType);
}

void    Camera::setFov(float fov)
{
    _fov = fov;
    _needUpdateProj = true;
}

void    Camera::setAspect(float aspect)
{
    _aspect = aspect;
    _needUpdateProj = true;
}

void    Camera::setNear(float near)
{
    _near = near;
    _needUpdateProj = true;
}

void    Camera::setFar(float far)
{
    _far = far;
    _needUpdateProj = true;
}

void    Camera::setDir(const glm::vec3& dir)
{
    _constants.dir = dir;
    _needUpdateView = true;
}

void    Camera::setScreen(const sScreen& screen)
{
    _screen = screen;
    _needUpdateProj = true;
}

void    Camera::setProjType(eProj projType)
{
    _projType = projType;
    _needUpdateProj = _needUpdateView = true;
}

void    Camera::translate(const glm::vec3& pos)
{
    _constants.pos += pos;
    _needUpdateView = true;
}

void    Camera::zoom(float amount)
{
    _zoom -= amount;

    // Limit max zoom to 40
    _zoom = std::max(_zoom, 0.1f);
    // Limit min zoom to 1.0f
    _zoom = std::min(_zoom, 0.5f);

    if (_projType == Camera::eProj::ORTHOGRAPHIC_2D ||
        _projType == Camera::eProj::ORTHOGRAPHIC_3D)
        _needUpdateProj = true;
    else
        _needUpdateView = true;
}

void    Camera::setZoom(float amount)
{
    _zoom = amount;
    if (_projType == Camera::eProj::ORTHOGRAPHIC_2D ||
        _projType == Camera::eProj::ORTHOGRAPHIC_3D)
        _needUpdateProj = true;
    else
        _needUpdateView = true;
}

void    Camera::update(const ShaderProgram& shaderProgram, float elapsedTime)
{
    float windowBufferWidth = (float)GameWindow::getInstance()->getBufferWidth();
    float windowBufferHeight = (float)GameWindow::getInstance()->getBufferHeight();

    // The window size changed
    if (_windowBufferSize.x != windowBufferWidth ||
        _windowBufferSize.y != windowBufferHeight)
    {
        // Calculate the scale between the previous size and the new size
        float scaleX = windowBufferWidth / _windowBufferSize.x;
        float scaleY = windowBufferHeight / _windowBufferSize.y;

        // Apply the scale to the camera screen and modify the projection matrice
        _screen.right *= scaleX;
        _screen.left *= scaleX;
        _screen.top *= scaleY;
        _screen.bottom *= scaleY;
        _needUpdateProj = true;

        _windowBufferSize.x = windowBufferWidth;
        _windowBufferSize.y = windowBufferHeight;

    }


    // Update matrix
    if (_needUpdateProj)
    {
        if (_projType == Camera::eProj::ORTHOGRAPHIC_3D)
        {
            _constants.proj = glm::ortho(_screen.left * _zoom, _screen.right * _zoom,
                                            _screen.bottom * _zoom, _screen.top * _zoom,
                                            _near, _far);
        }
        else if (_projType == Camera::eProj::ORTHOGRAPHIC_2D)
        {
            _constants.proj = glm::ortho(_screen.left * _zoom, _screen.right * _zoom,
                                            _screen.bottom * _zoom, _screen.top * _zoom,
                                            0.0f, _far);
        }
        else if (_projType == Camera::eProj::PERSPECTIVE)
        {
            _constants.proj = glm::perspective(_fov, _aspect, _near, _far);
        }
        else
            ASSERT(0, "Unknown projection type");

        _needUpdateProj = false;
        _needUpdateUbo = true;
    }
    if (_needUpdateView)
    {
        if (_projType == Camera::eProj::ORTHOGRAPHIC_3D)
        {
            _constants.view = glm::lookAt(_constants.pos, _constants.pos + _constants.dir, _up);
        }
        else if (_projType == Camera::eProj::PERSPECTIVE)
        {
            glm::vec3 newPos = _constants.pos - (glm::normalize(_constants.dir) * _zoom * 300.0f);
            _constants.view = glm::lookAt(newPos, newPos + _constants.dir, _up);
        }
        else if (_projType != Camera::eProj::ORTHOGRAPHIC_2D)
            ASSERT(0, "Unknown projection type");

        _needUpdateView = false;
        _needUpdateUbo = true;
    }

}

void    Camera::updateUboData(UniformBuffer& ubo, bool forceUpdate)
{
    if (_needUpdateUbo || forceUpdate)
        ubo.update(&_constants, sizeof(_constants));
    _needUpdateUbo = false;
}

void    Camera::freezeRotations(bool freeze)
{
    // Don't update ubo if nothing changed
    if (_constants.freezeRotations == static_cast<int>(freeze))
        return;

    _constants.freezeRotations = static_cast<int>(freeze);
    _needUpdateUbo = true;
}

Ray     Camera::screenPosToRay(float posX, float posY)
{
    auto gameWindow = GameWindow::getInstance();

    glm::vec3 nearScreen(posX, gameWindow->getBufferHeight() - posY, 0.0f);
    glm::vec3 farScreen(posX, gameWindow->getBufferHeight() - posY, 1.0f);

    // Unproject 2D points to get 3D points
    // Get 3D point on near plane
    glm::vec3 nearPoint = glm::unProject(nearScreen, getView(), getProj(), gameWindow->getViewport());
    // Get 3D point on far plane
    glm::vec3 farPoint = glm::unProject(farScreen, getView(), getProj(), gameWindow->getViewport());

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
