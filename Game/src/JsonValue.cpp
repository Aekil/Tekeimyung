#include "JsonValue.hpp"

JsonValue::JsonValue() {}

JsonValue::JsonValue(const Json::Value& json): _json(json) {}

JsonValue::JsonValue(const JsonValue& other)
{
    _json = other.get();
}

JsonValue::~JsonValue() {}

JsonValue&  JsonValue::operator=(const JsonValue& other)
{
    _json = other.get();
    return (*this);
}


JsonValue   JsonValue::get(const std::string& index, const Json::Value& defaultValue) const
{
    return JsonValue(_json.get(index, defaultValue));
}

Json::Value     JsonValue::get() const
{
    return _json;
}


bool    JsonValue::getBool(const std::string& index, bool defaultValue) const
{
    return _json.get(index, defaultValue).asBool();
}

int JsonValue::getInt(const std::string& index, int defaultValue) const
{
    return _json.get(index, defaultValue).asInt();
}

unsigned int    JsonValue::getUInt(const std::string& index, unsigned int defaultValue) const
{
    return _json.get(index, defaultValue).asUInt();
}

float   JsonValue::getFloat(const std::string& index, float defaultValue) const
{
    return _json.get(index, defaultValue).asFloat();
}

std::string JsonValue::getString(const std::string& index, const std::string& defaultValue) const
{
    return _json.get(index, defaultValue).asString();
}


glm::vec3   JsonValue::getColor3f(const std::string& index, const glm::vec3& defaultValue) const
{
    JsonValue color = get(index, {});

    return { color.getFloat("r", defaultValue.x), color.getFloat("g", defaultValue.y),
        color.getFloat("b", defaultValue.z) };
}

glm::vec4   JsonValue::getColor4f(const std::string& index, const glm::vec4& defaultValue) const
{
    JsonValue color = get(index, {});

    return { color.getFloat("r", defaultValue.x), color.getFloat("g", defaultValue.y),
        color.getFloat("b", defaultValue.z), color.getFloat("a", defaultValue.w) };
}


glm::vec2   JsonValue::getVec2f(const std::string& index, const glm::vec2& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat("x", defaultValue.x), vec.getFloat("y", defaultValue.y) };
}

glm::vec3   JsonValue::getVec3f(const std::string& index, const glm::vec3& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat("x", defaultValue.x), vec.getFloat("y", defaultValue.y),
    vec.getFloat("z", defaultValue.y) };
}

glm::vec4   JsonValue::getVec4f(const std::string& index, const glm::vec4& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat("x", defaultValue.x), vec.getFloat("y", defaultValue.y),
    vec.getFloat("z", defaultValue.y), vec.getFloat("w", defaultValue.y) };
}


glm::vec2   JsonValue::getVec2f(const std::string& index, const std::array<const char*, 2>& keys, const glm::vec2& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat(keys[0], defaultValue.x), vec.getFloat(keys[1], defaultValue.y) };
}

glm::vec3   JsonValue::getVec3f(const std::string& index, const std::array<const char*, 3>& keys, const glm::vec3& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat(keys[0], defaultValue.x), vec.getFloat(keys[1], defaultValue.y),
    vec.getFloat(keys[2], defaultValue.z) };
}

glm::vec4   JsonValue::getVec4f(const std::string& index, const std::array<const char*, 4>& keys, const glm::vec4& defaultValue) const
{
    JsonValue vec = get(index, {});

    return { vec.getFloat(keys[0], defaultValue.x), vec.getFloat(keys[1], defaultValue.y),
    vec.getFloat(keys[2], defaultValue.z), vec.getFloat(keys[3], defaultValue.w) };
}


uint32_t    JsonValue::size() const
{
    return _json.size();
}
