/**
* @Author   Guillaume Labey
*/

inline const glm::mat4& Transform::getTransform()
{
    if (_needUpdateTransform)
    {
        updateTransform();
    }
    return (_transform);
}

inline void    Transform::needUpdate()
{
    _needUpdateTransform = true;
    _dirty = true;
}

inline bool    Transform::isDirty()
{
    return (_dirty);
}

inline void    Transform::isDirty(bool dirty)
{
    _dirty = dirty;
}

inline const glm::vec3&    Transform::getScale() const
{
    return (_scale);
}

inline void    Transform::setScale(const glm::vec3& scale)
{
    _scale = scale;
    needUpdate();
}

inline const glm::vec3&    Transform::getPos() const
{
    return (_pos);
}

inline void    Transform::setPos(const glm::vec3& pos)
{
    _pos = pos;
    needUpdate();
}

inline const glm::vec3&    Transform::getRotation() const
{
    return (_rotation);
}

inline void    Transform::setRotation(const glm::vec3& rotation)
{
    _rotation = rotation;
    _needUpdateDirection = true;
    needUpdate();
}

inline const glm::quat& Transform::getOrientation()
{
    if (_needUpdateDirection)
    {
        updateDirection();
    }
    return (_orientation);
}

inline const glm::vec3& Transform::getDirection()
{
    if (_needUpdateDirection)
    {
        updateDirection();
    }
    return (_direction);
}

inline const glm::vec3& Transform::getUp()
{
    if (_needUpdateDirection)
    {
        updateDirection();
    }
    return (_up);
}

inline const glm::vec3& Transform::getRight()
{
    if (_needUpdateDirection)
    {
        updateDirection();
    }
    return (_right);
}
