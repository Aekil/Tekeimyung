#pragma once

#include "JsonValue.hpp"

class JsonReader
{
 public:
    JsonReader();
    ~JsonReader();

    bool parse(const std::string& document, JsonValue& json);
};
