/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Utils/JsonWriter.hpp>

JsonWriter::JsonWriter() {}

JsonWriter::~JsonWriter() {}

void    JsonWriter::write(const std::string& fileName, JsonValue& json)
{
    LOG_INFO("Saving json %s", fileName.c_str());
    RessourceManager::getInstance()->saveFile(fileName, json.get().toStyledString());
}
