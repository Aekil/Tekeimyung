/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform
{
public:
    enum class eTransform
    {
        LOCAL = 0,
        WORLD = 1
    };

public:
    Transform();
    ~Transform() = default;

    const glm::mat4&    getTransform();

    void                needUpdate();
    bool                isDirty();
    void                isDirty(bool dirty);
    const glm::vec3&    getScale() const;
    void                setScale(const glm::vec3& scale);
    const glm::vec3&    getPos() const;
    void                setPos(const glm::vec3& pos);
    const glm::vec3&    getRotation() const;
    void                setRotation(const glm::vec3& rotation);
    const glm::quat&    getOrientation();

    const glm::vec3&    getDirection();
    const glm::vec3&    getUp();
    const glm::vec3&    getRight();


    void                scale(const glm::vec3& scale);
    void                translate(const glm::vec3& direction, eTransform transform = eTransform::WORLD);
    void                rotate(float amount, const glm::vec3& axis);

private:
    void                updateDirection();
    void                updateTransform();

protected:
    glm::mat4           _transform = glm::mat4(1.0f);
    glm::vec3           _scale{1.0f, 1.0f, 1.0f};
    glm::vec3           _pos;

    // Up vector
    glm::vec3           _up{0.0f, 1.0f, 0.0f};
    // Right vector
    glm::vec3           _right{1.0f, 0.0f, 0.0f};

    // Euleur angles orientation
    // orientation.x = pitch
    // orientation.y = yaw
    // orientation.z = roll
    glm::vec3           _rotation;

    glm::quat           _orientation;

    // Direction vector
    glm::vec3           _direction;

    bool                _needUpdateTransform{true};
    bool                _needUpdateDirection{true};
    bool                _dirty{true};
};

#include <Engine/Graphics/Transform.inl>
