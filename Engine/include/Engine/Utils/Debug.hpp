/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <string>
#include <Engine/Utils/Helper.hpp>

// NDEBUG is defined for non-debug builds
// RELEASE_WITH_DEBUG is defined for RelWithDebInfo build (See CMakelists.txt in root directory)
#if !defined(NDEBUG) || defined(RELEASE_WITH_DEBUG)
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
