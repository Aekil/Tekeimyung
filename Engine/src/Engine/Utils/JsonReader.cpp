/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Utils/File.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/ResourceManager.hpp>

JsonReader::JsonReader() {}

JsonReader::~JsonReader() {}

bool    JsonReader::parse(const std::string& fileName, JsonValue& json)
{
    LOG_INFO("Parsing json \"%s\"", fileName.c_str());

    Json::Reader jsonReader;
    Json::Value parsed;

    std::string document = ResourceManager::getInstance()->getOrLoadResource<File>(fileName)->getContent();

    if (!jsonReader.parse(document, parsed))
    {
        std::cout << jsonReader.getFormattedErrorMessages() << std::endl;
        return (false);
    }

    json = parsed;
    return (true);
}
