/**
* @Author   Guillaume Labey
*/

#include <Game/Utils/JsonReader.hpp>

JsonReader::JsonReader() {}

JsonReader::~JsonReader() {}

bool    JsonReader::parse(const std::string& document, JsonValue& json)
{
    Json::Reader jsonReader;
    Json::Value parsed;

    if (!jsonReader.parse(document, parsed))
        return (false);

    json = parsed;
    return (true);
}
