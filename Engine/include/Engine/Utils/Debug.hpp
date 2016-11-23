#pragma once

#if defined(_DEBUG)
    #define ENGINE_DEBUG true

    #if defined(_MSC_VER)
        #define FUNCTION __FUNCSIG__
    #else
        #define FUNCTION __PRETTY_FUNCTION__
    #endif

#include <cstdlib>
    #define ASSERT(expression, message) \
        if (Debug::engineAssert(expression, message, __FILE__, FUNCTION, __LINE__) == false) \
            std::abort();
#else
    #define ENGINE_DEBUG false

    #define ASSERT(expression, message)
#endif

class           Debug
{
public:
    static bool engineAssert(bool expression, const char* message, const char* filename, const char* function, unsigned int line);
};
