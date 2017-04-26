/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <functional>

class   IInspectorImplementation
{
public:
    virtual         ~IInspectorImplementation() = default;
    virtual void    populateInspector() = 0;
};