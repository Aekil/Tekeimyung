#include <iostream>

#include <Engine/Utils/Debug.hpp>

bool    Debug::engineAssert(bool expression, const char* message, const char* filename,
                      const char* function, unsigned int line)
{
    #if defined (_DEBUG)
        if (expression == false)
        {
            std::cerr << "Assertion failed: \"" <<
                message << "\" in file " <<
                filename << ", at \"" <<
                function << "\" (line " <<
                line << ")" << std::endl;
            return (false);
        }
    #endif
    return (true);
}