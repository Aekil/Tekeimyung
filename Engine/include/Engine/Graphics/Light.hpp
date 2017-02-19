/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>

class Light
{
private:
    struct LightData{
        glm::vec3 ambient;
        float padding;
        glm::vec3 diffuse;
        float padding2;
        glm::vec3 direction;
    };

public:
    Light(const Light& light);
    Light();
    ~Light();

    Light&              operator=(const Light& light);

    const glm::vec3&    getAmbient() const;
    const glm::vec3&    getDiffuse() const;
    const glm::vec3&    getDirection() const;

    void                setAmbient(const glm::vec3& ambient);
    void                setDiffuse(const glm::vec3& diffuse);
    void                setDirection(const glm::vec3& direction);

    void                bind();

private:
    glm::vec3           _ambient;
    glm::vec3           _diffuse;
    glm::vec3           _direction;

    bool                _needUpdate;

    LightData           _data;

    UniformBuffer       _ubo;
};
