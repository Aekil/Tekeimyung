#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include "json/json.h"

class JsonValue
{
 public:
    JsonValue();
    JsonValue(const Json::Value& json);
    JsonValue(const JsonValue& other);
    ~JsonValue();

    JsonValue&                          operator=(const JsonValue& other);

    // Get any key
    JsonValue                           get(const std::string& index, const Json::Value& defaultValue) const;

    // Get jsoncpp object
    Json::Value                         get() const;

    // Get scalar/string/bool
    bool                                getBool(const std::string& index, bool defaultValue) const;
    int                                 getInt(const std::string& index, int defaultValue) const;
    unsigned int                        getUInt(const std::string& index, unsigned int defaultValue) const;
    float                               getFloat(const std::string& index, float defaultValue) const;
    std::string                         getString(const std::string& index, const std::string& defaultValue) const;

    // Get colors
    glm::vec3                           getColor3f(const std::string& index, const glm::vec3& defaultValue) const;
    glm::vec4                           getColor4f(const std::string& index, const glm::vec4& defaultValue) const;

    // Get vectors
    glm::vec2                           getVec2f(const std::string& index, const glm::vec2& defaultValue) const;
    glm::vec3                           getVec3f(const std::string& index, const glm::vec3& defaultValue) const;
    glm::vec4                           getVec4f(const std::string& index, const glm::vec4& defaultValue) const;

    // Get vectors with specific keys
    glm::vec2                           getVec2f(const std::string& index, const std::array<const char*, 2>& keys, const glm::vec2& defaultValue) const;
    glm::vec3                           getVec3f(const std::string& index, const std::array<const char*, 3>& keys, const glm::vec3& defaultValue) const;
    glm::vec4                           getVec4f(const std::string& index, const std::array<const char*, 4>& keys, const glm::vec4& defaultValue) const;


    // Set any value to key
    void                                setValue(const std::string& index, const JsonValue& value);

    // Set scalar/string/bool
    void                                setBool(const std::string& index, bool value);
    void                                setInt(const std::string& index, int value);
    void                                setUInt(const std::string& index, unsigned int value);
    void                                setFloat(const std::string& index, float value);
    void                                setString(const std::string& index, const std::string& value);
    void                                setStringVec(const std::string& index, const std::vector<std::string>& value);

    // Set colors
    void                                setColor3f(const std::string& index, const glm::vec3& value);
    void                                setColor4f(const std::string& index, const glm::vec4& value);

    // Set vectors
    void                                setVec2f(const std::string& index, const glm::vec2& value);
    void                                setVec3f(const std::string& index, const glm::vec3& value);
    void                                setVec4f(const std::string& index, const glm::vec4& value);

    // Set vectors with specific keys
    void                                setVec2f(const std::string& index, const std::array<const char*, 2>& keys, const glm::vec2& value);
    void                                setVec3f(const std::string& index, const std::array<const char*, 3>& keys, const glm::vec3& value);
    void                                setVec4f(const std::string& index, const std::array<const char*, 4>& keys, const glm::vec4& value);


    uint32_t                            size() const;

private:
    Json::Value                         _json;
};
