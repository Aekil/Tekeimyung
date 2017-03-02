#include <Engine/Graphics/Transform.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
{
    updateDirection();
}

void    Transform::updateTransform()
{
    glm::quat newRotate(glm::vec3(glm::radians(_rotation.x), glm::radians(_rotation.y), glm::radians(_rotation.z)));

    _needUpdate = false;
    glm::mat4 newTranslate = glm::translate(glm::mat4(1.0), glm::vec3(_pos.x, _pos.y, _pos.z));
    glm::mat4 newScale = glm::scale(glm::mat4(1.0), _scale);
    _transform = newTranslate * glm::mat4_cast(newRotate) * newScale;
}

const glm::mat4&    Transform::getTransform()
{
    if (_needUpdate)
    {
        updateTransform();
    }
    return (_transform);
}

void    Transform::scale(const glm::vec3& scale)
{
    _scale += scale;
    needUpdate();
}

void    Transform::translate(const glm::vec3& direction, eTransform transform)
{
    if (transform == eTransform::LOCAL)
    {
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

    // Handle screen flipping
    // TODO: Use quaternions
    if (_rotation.x > 89.0f)
    {
        _rotation.x = 89.0f;
    }
    else if (_rotation.x < -89.0f)
    {
        _rotation.x = -89.0f;
    }

    updateDirection();
    needUpdate();
}

void    Transform::updateDirection()
{
    _direction.x = cos(glm::radians(_rotation.x)) * cos(glm::radians(_rotation.y));
    _direction.y = sin(glm::radians(_rotation.x));
    _direction.z = cos(glm::radians(_rotation.x)) * sin(glm::radians(_rotation.y));
    _direction = glm::normalize(_direction);

    _right = glm::normalize(glm::cross(_direction, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))));
    _up = glm::normalize(glm::cross(_right, _direction));
}
