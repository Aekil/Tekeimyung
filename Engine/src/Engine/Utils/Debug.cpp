#include <iostream>

#include <Engine/Utils/Debug.hpp>

bool    Debug::engineAssert(bool expression, std::string message, const char* filename,
                      const char* function, unsigned int line)
{
        if (expression == false)
        {
            std::cerr << "Assertion failed: \"" <<
                message << "\" in file " <<
                filename << ", at \"" <<
                function << "\" (line " <<
                line << ")" << std::endl;
            return (false);
        }
    return (true);
}
