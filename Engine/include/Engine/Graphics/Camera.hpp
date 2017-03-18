/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <Engine/Graphics/Ray.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Transform.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Utils/Helper.hpp>

#define PROJECTION_TYPES(PROCESS)\
    PROCESS(PERSPECTIVE)\
    PROCESS(ORTHOGRAPHIC_3D)\
    PROCESS(ORTHOGRAPHIC_2D)\

class Camera: public Transform
{
public:
    struct sConstants
    {
        glm::mat4       proj;
        glm::mat4       view;
        glm::vec3       pos;
        float           padding;
        // Target
        glm::vec3       dir;
    };

    struct sViewport
    {
        struct sOffset
        {
            float           x;
            float           y;
        } offset;
        struct sExtent
        {
            float           width;
            float           height;
        } extent;
    };

    REGISTER_ENUM(eProj, uint8_t, PROJECTION_TYPES)

public:
    Camera();
    ~Camera();

    const glm::mat4&    getView();
    const glm::mat4&    getProj();
    float               getAspect() const;
    float               getFov() const;
    float               getFar() const;
    float               getNear() const;
    eProj               getProjType() const;
    float               getProjSize() const;
    const sViewport&    getViewportRect() const;
    const sViewport&    getViewport() const;

    void                setFov(float fov);
    void                setNear(float near);
    void                setFar(float far);
    void                setViewportRect(const sViewport& viewportRect);
    void                setProjType(eProj projType);
    void                setProjSize(float projSize);

    void                lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

    void                updateViewport();
    void                updateUBO();
    UniformBuffer&      getUBO();

    void                freezeRotations(bool freeze);

    Ray                 screenPosToRay(float posX, float posY);

    static void         setInstance(Camera* instance);
    static Camera*      getInstance();

    // Size of visible ortho projection
    glm::vec3           getVisibleAreaSize(const glm::vec3& projSize) const;

private:
    void                updateProj();
    void                updateView();

private:
    sConstants          _constants;


    UniformBuffer       _ubo;

    // Update proj/view if true
    bool                _needUpdateView;
    bool                _needUpdateProj;

    // Field of view
    float               _fov;
    // Near clipping plane
    float               _near;
    // Far clipping plane
    float               _far;

    // Projection size for ORTHOGRAPHIC_3D
    float               _projSize;

    // Viewport values between 0 and 1
    sViewport           _viewportRect;
    // Viewport real values
    sViewport           _viewport;

    eProj               _projType;

    static Camera*      _instance;

    glm::mat4           _proj;
    glm::mat4           _view;
};

REGISTER_ENUM_MANAGER(Camera::eProj, PROJECTION_TYPES)
