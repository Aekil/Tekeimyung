/**
* @Author   Guillaume Labey
*/

#include <GL/glew.h>

#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/EnumManager.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/File.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>

ShaderProgram::ShaderProgram()
{
    // Create shader program
    _shaderProgram = glCreateProgram();
    _options = 0;
    _linked = false;
}

ShaderProgram::~ShaderProgram()
{
    // Delete shaders
    for (auto &&shader: _shaders)
    {
        glDeleteShader(shader.first);
    }
}

void    ShaderProgram::attachShader(GLenum shaderType, const std::string& fileName, const std::vector<Material::eOption>& options)
{
    // Get shader raw source code
    std::string shaderString = ResourceManager::getInstance()->getOrLoadResource<File>(fileName)->getContent();

    // Define shader options
    {
        for (auto& option: options)
        {
            std::string optionString = EnumManager<Material::eOption>::enumToString(option);
            optionString.insert(0, "#define ");
            optionString.append("\n");
            shaderString.insert(0, optionString);
        }
    }

    shaderString.insert(0, "#version 420 core\n");

    // Create shader and compiles it
    const char *cShaderString = shaderString.c_str();
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
        EXCEPT(RendererAPIException, "Failed to compile shader \"%s\" from file \"%s\"\nError: %s", shaderTypeName.c_str(), fileName.c_str(), infoLog);
    }

    // Attach shaders to shader program
    glAttachShader(_shaderProgram, _shaders[shaderType]);
}

void    ShaderProgram::link()
{
    ASSERT(_linked == false, "A ShaderProgram should not be linked 2 times");

    // Attach shaders in one final shader program
    glLinkProgram(_shaderProgram);

    checkProgramError();

    _linked = true;
}

void    ShaderProgram::setOptions(int options)
{
    _options = options;
}

int     ShaderProgram::getOptions() const
{
    return (_options);
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
        EXCEPT(RendererAPIException, "Failed to link shaders to shader program. Error: %s", infoLog);
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

GLuint  ShaderProgram::getUniformLocation(const char* location)
{
    auto uniform = _uniformLocations.find(location);
    if (uniform == _uniformLocations.end())
    {
        _uniformLocations[location] = glGetUniformLocation(_shaderProgram, location);
        return _uniformLocations.at(location);
    }

    return (uniform->second);
}

GLuint  ShaderProgram::getUniformBlockIndex(const char* name) const
{
    return (glGetUniformBlockIndex(_shaderProgram, name));
}
