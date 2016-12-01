#pragma once

#include <string>
#include <Engine/Utils/Helper.hpp>

#if defined(_DEBUG)
    #define ENGINE_DEBUG true

    #if defined(_MSC_VER)
        #define FUNCTION __FUNCSIG__
    #else
        #define FUNCTION __PRETTY_FUNCTION__
    #endif

#include <cstdlib>
    #define ASSERT(expression, format, ...) \
        if (expression == false && Debug::engineAssert(expression, Helper::formatMessage(format, ## __VA_ARGS__), __FILE__, FUNCTION, __LINE__) == false) \
            std::abort();
#else
    #define ENGINE_DEBUG false

    #define ASSERT(expression, format, ...)
#endif

class           Debug
{
public:
    static bool engineAssert(bool expression, std::string message, const char* filename, const char* function, unsigned int line);
};
