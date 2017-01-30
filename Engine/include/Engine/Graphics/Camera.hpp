/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

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
        int             freezeRotations;
        // Target
        glm::vec3       dir;
    }                   Constants;

    typedef struct
    {
        float           left;
        float           right;
        float           top;
        float           bottom;
    }                   sScreen;

    enum class eProj: uint8_t
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC_3D = 1,
        ORTHOGRAPHIC_2D = 2
    };

public:
    Camera();
    ~Camera();

    bool                needUpdate() const;

    const glm::vec3&    getPos() const;
    const glm::mat4&    getView() const;
    const glm::mat4&    getProj() const;
    float               getAspect() const;
    float               getFov() const;
    eProj               getProjType() const;

    void                setFov(float fov);
    void                setAspect(float aspect);
    void                setNear(float near);
    void                setFar(float far);
    void                setDir(const glm::vec3& dir);
    void                setScreen(const sScreen& screen);
    void                setProjType(eProj projType);

    void                translate(const glm::vec3& pos);
    void                zoom(float amount);
    void                setZoom(float amount);

    void                lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

    void                update(const ShaderProgram& shaderProgram, float elapsedTime);
    void                updateUboData(UniformBuffer& ubo, bool forceUpdate = false);
    void                freezeRotations(bool freeze);

    static void         setInstance(Camera* instance);
    static Camera*      getInstance();

private:
    Camera::Constants   _constants;

    bool                _needUpdateView;
    bool                _needUpdateProj;
    bool                _needUpdateUbo;

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

    float               _zoom;

    sScreen             _screen;

    eProj               _projType;

    /*
    ** View
    */
    // Up vector
    glm::vec3           _up;

    glm::vec3           _windowBufferSize;

    static Camera*      _instance;
};
