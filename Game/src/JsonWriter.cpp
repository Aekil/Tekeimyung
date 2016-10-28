#include "Utils/RessourceManager.hpp"

#include "JsonWriter.hpp"

JsonWriter::JsonWriter() {}

JsonWriter::~JsonWriter() {}

void    JsonWriter::write(const std::string& fileName, JsonValue& json)
{
    RessourceManager::getInstance()->saveFile(fileName, json.get().toStyledString());
}
