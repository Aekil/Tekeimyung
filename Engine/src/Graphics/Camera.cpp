#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window/GameWindow.hpp"

#include "Graphics/Camera.hpp"

Camera::Camera(): _needUpdateView(false), _needUpdateProj(true), _fov(45.0f), _aspect(1280.0f / 960.0f), _near(0.1f), _far(1000.0f), _up({0.0f, 1.0f, 0.0f})
{
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

const UniformBuffer&    Camera::getUbo() const
{
    return _ubo;
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
    _dir = dir;
    _needUpdateView = true;
}

void    Camera::translate(const glm::vec3& pos)
{
    _constants.pos += pos;
    _needUpdateView = true;
}

void    Camera::zoom(float amount)
{
    _constants.pos.y -= amount;

    // Limit max zoom to 40
    _constants.pos.y = std::max(_constants.pos.y, 40.0f);
    // Limit min zoom to 300
    _constants.pos.y = std::min(_constants.pos.y, 300.0f);
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
        translate(glm::vec3(40.0f * elapsedTime, 0.0f, -40.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Q))
        translate(glm::vec3(-40.0f * elapsedTime, 0.0f, 40.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Z))
        translate(glm::vec3(-40.0f * elapsedTime, 0.0f, -40.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::S))
        translate(glm::vec3(40.0f * elapsedTime, 0.0f, 40.0f * elapsedTime));

    // Update zoom
    double offset = scroll.yOffset - lastScrollOffset;

    if (offset)
        zoom((float)(-offset * elapsedTime * 500.0f));
    lastScrollOffset = scroll.yOffset;

    // Update matrix
    if (_needUpdateProj)
    {
        _constants.proj = glm::perspective(_fov, _aspect, _near, _far);
        _needUpdateProj = false;
    }
    if (_needUpdateView)
    {
        _constants.view = glm::lookAt(_constants.pos, _constants.pos + _dir, _up);
        _needUpdateView = false;
    }

    _ubo.update(&_constants, sizeof(_constants));
}
