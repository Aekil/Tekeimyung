#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdint>
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

    uint32_t                            size() const;

private:
    Json::Value                         _json;
};
