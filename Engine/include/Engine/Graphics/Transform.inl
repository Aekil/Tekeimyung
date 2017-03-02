inline void    Transform::needUpdate()
{
    _needUpdate = true;
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
    needUpdate();
}

inline const glm::vec3& Transform::getDirection() const
{
    return (_direction);
}

inline const glm::vec3& Transform::getUp() const
{
    return (_up);
}

inline const glm::vec3& Transform::getRight() const
{
    return (_right);
}
