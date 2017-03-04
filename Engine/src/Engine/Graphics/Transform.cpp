#include <Engine/Graphics/Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform() {}

void    Transform::scale(const glm::vec3& scale)
{
    _scale += scale;
    needUpdate();
}

void    Transform::translate(const glm::vec3& direction, eTransform transform)
{
    if (transform == eTransform::LOCAL)
    {
        if (_needUpdateDirection)
        {
            updateDirection();
        }

        // TODO: rotate direction with _orientation vector
        // (Find why it does not work)
        if (direction.x != 0.0f)
        {
            _pos += _right * direction.x;
        }
        if (direction.y != 0.0f)
        {
            _pos += _up * direction.y;
        }
        if (direction.z != 0.0f)
        {
            _pos += _direction * direction.z;
        }
    }
    else
    {
        _pos += direction;
    }

    needUpdate();
}

void    Transform::rotate(float amount, const glm::vec3& axis)
{
    if (axis.x == 1.0f)
        _rotation.x += amount;
    if (axis.y == 1.0f)
        _rotation.y += amount;
    if (axis.z == 1.0f)
        _rotation.z += amount;

    needUpdate();
    _needUpdateDirection = true;
}

void    Transform::updateDirection()
{
    // TODO: Use rollQuat (need gimbal lock handle)
    glm::quat pitchQuat = glm::angleAxis(glm::radians(-_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yawQuat = glm::angleAxis(glm::radians(-_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rollQuat = glm::angleAxis(glm::radians(-_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _orientation = pitchQuat * yawQuat * rollQuat;
    _orientation = glm::normalize(_orientation);

    glm::mat4 rotate = glm::mat4_cast(_orientation);
    _up = glm::normalize(glm::vec3(rotate[0][1], rotate[1][1], rotate[2][1]));
    _right = glm::normalize(glm::vec3(rotate[0][0], rotate[1][0], rotate[2][0]));
    _direction = glm::normalize(glm::vec3(rotate[0][2], rotate[1][2], rotate[2][2]));

    _needUpdateDirection = false;
}

void    Transform::updateTransform()
{
    // TODO: Call getOrientation() to get orientation
    // (Not working because _rotation.x need to be inversed)
    glm::quat rotationQuat(glm::vec3(glm::radians(_rotation.x), glm::radians(_rotation.y), glm::radians(_rotation.z)));

    _needUpdateTransform = false;
    glm::mat4 newTranslate = glm::translate(glm::mat4(1.0), _pos);
    glm::mat4 newScale = glm::scale(glm::mat4(1.0), _scale);
    _transform = newTranslate * glm::mat4_cast(rotationQuat) * newScale;
}
