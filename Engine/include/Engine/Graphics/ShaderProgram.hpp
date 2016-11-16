#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void                                    attachShader(GLenum shaderType, const std::string& fileName);
    void                                    link();
    void                                    use();

    GLuint                                  getUniformLocation(const char* location) const;
    GLuint                                  getUniformBlockIndex(const char* name) const;

    void                                    bindBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const;

private:
    std::string                             shaderNameFromType(GLenum shaderType) const;
    void                                    checkProgramError() const;


private:
    std::unordered_map<GLenum, GLuint>      _shaders;
    GLuint                                  _shaderProgram;
};
