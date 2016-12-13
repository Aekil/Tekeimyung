/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/RessourceManager.hpp>

#include <Game/Utils/JsonWriter.hpp>

JsonWriter::JsonWriter() {}

JsonWriter::~JsonWriter() {}

void    JsonWriter::write(const std::string& fileName, JsonValue& json)
{
    RessourceManager::getInstance()->saveFile(fileName, json.get().toStyledString());
}
