#pragma once

#include <typeinfo>

struct sComponent
{
    virtual const std::type_info& getTypeInfo()
    {
        return (typeid(*this));
    }
};