#pragma once

#include <typeinfo>

struct Component
{
    virtual const std::type_info &getTypeInfo()
    {
        return typeid(*this);
    }
};