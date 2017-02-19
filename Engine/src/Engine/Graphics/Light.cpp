/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <Engine/Graphics/Light.hpp>

Light::Light(const Light& light)
{
    _ambient = light._ambient;
    _diffuse = light._diffuse;
    _direction = light._direction;
    _needUpdate = true;
    _ubo.init(sizeof(LightData));
    _ubo.setBindingPoint(2);
}

Light::Light(): _needUpdate(true)
{
    _ubo.init(sizeof(LightData));
    _ubo.setBindingPoint(2);
    _ambient = {0.3f, 0.3f, 0.3f};
    _diffuse = {1.0f, 1.0f, 1.0f};
    _direction = {0.0f, -1.0f, 0.0f};
}

Light::~Light() {}

Light&  Light::operator=(const Light& light)
{
    _ambient = light._ambient;
    _diffuse = light._diffuse;
    _direction = light._direction;
    _needUpdate = true;
    _ubo.init(sizeof(LightData));
    _ubo.setBindingPoint(2);

    return (*this);
}

const glm::vec3&    Light::getAmbient() const
{
    return (_ambient);
}
const glm::vec3&    Light::getDiffuse() const
{
    return (_diffuse);
}
const glm::vec3&    Light::getDirection() const
{
    return (_direction);
}

void    Light::setAmbient(const glm::vec3& ambient)
{
    _ambient = ambient;
    _needUpdate = true;
}
void    Light::setDiffuse(const glm::vec3& diffuse)
{
    _diffuse = diffuse;
    _needUpdate = true;
}
void    Light::setDirection(const glm::vec3& direction)
{
    _direction = direction;
    _needUpdate = true;
}

void    Light::bind()
{
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.direction = _direction;
        _needUpdate = false;

        _ubo.update(&_data, sizeof(LightData));
    }

    _ubo.bind();
}
