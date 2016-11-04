#pragma once

#include <glm/vec4.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    bool                needUpdate() const;

    const glm::mat4&    getProj() const;
    const glm::mat4&    getView() const;
    const glm::vec3&    getPos() const;

    void                setFov(float fov);
    void                setAspect(float aspect);
    void                setNear(float near);
    void                setFar(float far);
    void                setDir(const glm::vec3& dir);

    void                translate(const glm::vec3& pos);

    void                lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

    void                update(float elapsedTime);

private:
    glm::mat4           _view;
    glm::mat4           _proj;

    bool                _needUpdateView;
    bool                _needUpdateProj;

    /*
    ** Projection
    */
    // Field of view
    float               _fov;
    // Aspect ratio. Ex: 1920/1080
    float               _aspect;
    // Near clipping plane
    float               _near;
    // Far clipping plane
    float               _far;

    /*
    ** View
    */
    // Position
    glm::vec3           _pos;
    // Target
    glm::vec3           _dir;
    // Up vector
    glm::vec3           _up;
};
