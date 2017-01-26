/**
* @Author   Guillaume Labey
*/

#include <iostream>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/RessourceManager.hpp>

JsonReader::JsonReader() {}

JsonReader::~JsonReader() {}

bool    JsonReader::parse(const std::string& fileName, JsonValue& json)
{
    LOG_INFO("Loading json \"%s\"", fileName.c_str());

    Json::Reader jsonReader;
    Json::Value parsed;

    std::string document = RessourceManager::getInstance()->getFile(fileName);

    if (!jsonReader.parse(document, parsed))
    {
        std::cout << jsonReader.getFormattedErrorMessages() << std::endl;
        return (false);
    }

    json = parsed;
    return (true);
}
