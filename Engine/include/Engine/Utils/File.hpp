/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>

#include <Engine/Utils/Resource.hpp>

class File: public Resource
{
public:
    File();
    File(const std::string& path);
    ~File();

    bool                        loadFromFile(const std::string& fileName) override final;
    void                        save();

    const std::string&          getContent() const;
    void                        setContent(const std::string& content);

    static Resource::eType      getResourceType() { return Resource::eType::FILE; }

private:
    std::string                 _content;
};
