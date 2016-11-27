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

Camera::Camera(): _needUpdateView(false), _needUpdateProj(true), _fov(45.0f),
                    _aspect(1920.0f / 1080.0f), _near(0.1f), _far(1000.0f),
                    _up({0.0f, 1.0f, 0.0f}), _zoom(0.5f), _proj(Camera::eProj::ORTHOGRAPHIC)
{
    _constants.freezeRotations = 0;
    _ubo.setBindingPoint(1);
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

const UniformBuffer&    Camera::getUbo() const
{
    return (_ubo);
}

float   Camera::getAspect() const
{
    return (_aspect);
}

float   Camera::getFov() const
{
    return (_fov);
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

    if (_proj == Camera::eProj::ORTHOGRAPHIC)
        _needUpdateProj = true;
    else
        _needUpdateView = true;
}

void    Camera::update(const ShaderProgram& shaderProgram, float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();
    auto &&scroll = mouse.getScroll();
    static double lastScrollOffset;

    // Update position
    if (keyboard.isPressed(Keyboard::eKey::D))
        translate(glm::vec3(60.0f * elapsedTime, 0.0f, -60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Q))
        translate(glm::vec3(-60.0f * elapsedTime, 0.0f, 60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Z))
        translate(glm::vec3(-60.0f * elapsedTime, 0.0f, -60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::S))
        translate(glm::vec3(60.0f * elapsedTime, 0.0f, 60.0f * elapsedTime));

    // Update Projection type
    if (keyboard.isPressed(Keyboard::eKey::O))
    {
        _proj = Camera::eProj::ORTHOGRAPHIC;
        _needUpdateProj = _needUpdateView = true;
    }
    if (keyboard.isPressed(Keyboard::eKey::P))
    {
        _proj = Camera::eProj::PERSPECTIVE;
        _needUpdateProj = _needUpdateView = true;
    }

    // Update zoom
    double offset = scroll.yOffset - lastScrollOffset;

    if (offset)
        zoom((float)(-offset * elapsedTime));
    lastScrollOffset = scroll.yOffset;

    // Update matrix
    if (_needUpdateProj)
    {
        if (_proj == Camera::eProj::ORTHOGRAPHIC)
        {
            _constants.proj = glm::ortho(_screen.left * _zoom, _screen.right * _zoom,
                                            _screen.bottom * _zoom, _screen.top * _zoom,
                                            _near, _far);
        }
        else if (_proj == Camera::eProj::PERSPECTIVE)
        {
            _constants.proj = glm::perspective(_fov, _aspect, _near, _far);
        }
        else
            ASSERT(0, "Unknown projection type");

        _needUpdateProj = false;
    }
    if (_needUpdateView)
    {
        if (_proj == Camera::eProj::ORTHOGRAPHIC)
        {
            _constants.view = glm::lookAt(_constants.pos, _constants.pos + _constants.dir, _up);
        }
        else if (_proj == Camera::eProj::PERSPECTIVE)
        {
            glm::vec3 newPos = _constants.pos - (glm::normalize(_constants.dir) * _zoom * 300.0f);
            _constants.view = glm::lookAt(newPos, newPos + _constants.dir, _up);
        }
        else
            ASSERT(0, "Unknown projection type");

        _needUpdateView = false;
    }

    _ubo.update(&_constants, sizeof(_constants));
}

void    Camera::freezeRotations(bool freeze)
{
    // Don't update ubo if nothing changed
    if (_constants.freezeRotations == static_cast<int>(freeze))
        return;

    _constants.freezeRotations = static_cast<int>(freeze);
    _ubo.update(&_constants, sizeof(_constants));
}

void    Camera::setInstance(Camera* instance)
{
    _instance = instance;
}

Camera* Camera::getInstance()
{
    return (_instance);
}
