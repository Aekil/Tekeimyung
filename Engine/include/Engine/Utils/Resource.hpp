/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>

class Resource
{
public:
    enum class eType: uint8_t
    {
        MODEL = 0,
        MATERIAL = 1,
        GEOMETRY = 2,
        FILE = 3,
        TEXTURE = 4
    };

public:
    Resource();
    ~Resource();

    const std::string   getId() const;
    void                setId(const std::string& id);

    const std::string   getPath() const;
    void                setPath(const std::string& path);

    virtual bool        loadFromFile(const std::string& fileName) = 0;

private:
    std::string         _id;
    std::string         _path;
};
