#include <GL/glew.h>
#include "Graphics/ShaderProgram.hpp"
#include "Utils/RessourceManager.hpp"
#include "Utils/Exception.hpp"

ShaderProgram::ShaderProgram()
{
    // Create shader program
    _shaderProgram = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    // Delete shaders
    for (auto &&shader: _shaders)
    {
        glDeleteShader(shader.first);
    }
}

void    ShaderProgram::attachShader(GLenum shaderType, const std::string& fileName)
{
    std::string         shaderString;

    // Get shader raw source code
    shaderString = RessourceManager::getInstance()->getFile(fileName);
    const char *cShaderString = shaderString.c_str();

    // Create shader and compiles it
    _shaders[shaderType] = glCreateShader(shaderType);
    glShaderSource(_shaders[shaderType], 1, &cShaderString, NULL);
    glCompileShader(_shaders[shaderType]);

    // Check for compilation was successful
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(_shaders[shaderType], GL_COMPILE_STATUS, &success);

    // If compilation did not succeed, get the error message and throw an Exception
    if (!success)
    {
        std::string shaderTypeName = shaderNameFromType(shaderType);
        glGetShaderInfoLog(_shaders[shaderType], 512, NULL, infoLog);
        EXCEPT(RendererAPIException, "Failed to compile shader \"%s\"", shaderTypeName.c_str());
    }

    // Attach shaders to shader program
    glAttachShader(_shaderProgram, _shaders[shaderType]);
}

void    ShaderProgram::link()
{
    // Attach shaders in one final shader program
    glLinkProgram(_shaderProgram);

    checkProgramError();
}

void    ShaderProgram::checkProgramError() const
{
    GLint success;
    GLchar infoLog[512];

    // Check errors on program
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);

    // If compilation did not succeed, get the error message and throw an Exception
    if(!success)
    {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        EXCEPT(RendererAPIException, "Failed to attach shaders to shader program");
    }
}

std::string     ShaderProgram::shaderNameFromType(GLenum shaderType) const
{
    std::unordered_map<GLenum, std::string>  shaderTypes;

    shaderTypes = {
        {GL_VERTEX_SHADER, "VERTEX"},
        {GL_FRAGMENT_SHADER, "FRAGMENT"}
    };

    try
    {
        return shaderTypes.at(shaderType);
    }
    catch(...)
    {
        return "";
    }
}

void    ShaderProgram::use()
{
    glUseProgram(_shaderProgram);
}

GLuint  ShaderProgram::getUniformLocation(const char* location) const
{
    return glGetUniformLocation(_shaderProgram, location);
}
