/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Resource.hpp>

Resource::Resource() {}

Resource::Resource(const Resource& resource)
{
    _path = resource._path;
    _id = resource._id;
}

Resource::~Resource() {}

Resource&   Resource::operator=(const Resource& resource)
{
    _path = resource._path;
    _id = resource._id;
    return (*this);
}

const std::string   Resource::getId() const
{
    return (_id);
}

void    Resource::setId(const std::string& id)
{
    _id = id;
}

const std::string   Resource::getPath() const
{
    return (_path);
}

void    Resource::setPath(const std::string& path)
{
    _path = path;
}
