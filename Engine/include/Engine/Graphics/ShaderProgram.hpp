/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <Engine/Graphics/Material.hpp>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void                                    attachShader(GLenum shaderType, const std::string& fileName, const std::vector<Material::eOption>& options = {});
    void                                    link();
    void                                    use();

    GLuint                                  getUniformLocation(const char* location);
    GLuint                                  getUniformBlockIndex(const char* name) const;

    void                                    setOptions(int options);
    int                                     getOptions() const;

private:
    std::string                             shaderNameFromType(GLenum shaderType) const;
    void                                    checkProgramError() const;


private:
    std::unordered_map<GLenum, GLuint>      _shaders;
    std::unordered_map<std::string, GLuint>     _uniformLocations;
    GLuint                                  _shaderProgram;
    int                                     _options;

    bool                                    _linked;
};
