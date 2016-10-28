#pragma once

#include "Utils/JsonValue.hpp"

class JsonReader
{
 public:
    JsonReader();
    ~JsonReader();

    bool parse(const std::string& document, JsonValue& json);
};
