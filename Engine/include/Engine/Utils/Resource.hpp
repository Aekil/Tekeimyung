#pragma once

#include <string>

class Resource
{
public:
    Resource();
    ~Resource();

    const std::string   getId() const;
    void                setId(const std::string& id);

    const std::string   getPath() const;
    void                setPath(const std::string& path);

private:
    std::string         _id;
    std::string         _path;
};
