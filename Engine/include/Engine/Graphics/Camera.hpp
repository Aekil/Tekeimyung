#pragma once

#include <glm/vec4.hpp>

#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

class Camera
{
public:
    typedef struct
    {
        glm::mat4       proj;
        glm::mat4       view;
        glm::vec3       pos;
    }                   Constants;

public:
    Camera();
    ~Camera();

    bool                needUpdate() const;

    const glm::vec3&    getPos() const;
    const UniformBuffer& getUbo() const;

    void                setFov(float fov);
    void                setAspect(float aspect);
    void                setNear(float near);
    void                setFar(float far);
    void                setDir(const glm::vec3& dir);

    void                translate(const glm::vec3& pos);
    void                zoom(float amount);

    void                lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

    void                update(const ShaderProgram& shaderProgram, float elapsedTime);

private:
    Camera::Constants   _constants;
    UniformBuffer       _ubo;

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
    // Target
    glm::vec3           _dir;
    // Up vector
    glm::vec3           _up;
};
