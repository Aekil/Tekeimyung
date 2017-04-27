/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/File.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Utils/JsonWriter.hpp>

JsonWriter::JsonWriter() {}

JsonWriter::~JsonWriter() {}

void    JsonWriter::write(const std::string& fileName, JsonValue& json)
{
    File* file = ResourceManager::getInstance()->getOrCreateResource<File>(fileName);
    if (!file)
    {
        LOG_ERROR("Failed to get json resource %s", fileName.c_str());
        return;
    }

    file->setContent(json.get().toStyledString());
    file->save();
}
