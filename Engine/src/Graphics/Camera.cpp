#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window/GameWindow.hpp"

#include "Graphics/Camera.hpp"

Camera::Camera(): _needUpdateView(false), _needUpdateProj(true), _fov(45.0f), _aspect(1280.0f / 960.0f), _near(0.1f), _far(1000.0f), _up({0.0f, 1.0f, 0.0f}) {}

Camera::~Camera() {}

bool    Camera::needUpdate() const
{
    return (_needUpdateProj || _needUpdateView);
}

const glm::mat4&    Camera::getProj() const
{
    return (_proj);
}

const glm::mat4&    Camera::getView() const
{
    return (_view);
}

const glm::vec3&    Camera::getPos() const
{
    return (_pos);
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
    _pos += pos;
    _needUpdateView = true;
}

void    Camera::zoom(float amount)
{
    _pos.y -= amount;
    _needUpdateView = true;
}

void    Camera::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();
    auto &&cursor = mouse.getCursor();
    static glm::vec2 lastMousePos;

    // Update position
    if (keyboard.isPressed(Keyboard::eKey::D))
        translate(glm::vec3(20.0f * elapsedTime, 0.0f, -20.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Q))
        translate(glm::vec3(-20.0f * elapsedTime, 0.0f, 20.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Z))
        translate(glm::vec3(-20.0f * elapsedTime, 0.0f, -20.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::S))
        translate(glm::vec3(20.0f * elapsedTime, 0.0f, 20.0f * elapsedTime));

    // Update zoom
    glm::vec2 mousePos = { cursor.getX(), cursor.getY() };
    if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1))
    {
        float move = (mousePos.x - lastMousePos.x) + (mousePos.y - lastMousePos.y);

        if (move)
            zoom(move * elapsedTime * 4.0f);
    }
    lastMousePos = mousePos;

    // Update matrix
    if (_needUpdateProj)
    {
        _proj = glm::perspective(_fov, _aspect, _near, _far);
        _needUpdateProj = false;
    }
    if (_needUpdateView)
    {
        _view = glm::lookAt(_pos, _pos + _dir, _up);
        _needUpdateView = false;
    }
}
