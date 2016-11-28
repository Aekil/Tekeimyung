#pragma once

#include <fstream>
#include <memory>
#include <string>

#include <Engine/Utils/Debug.hpp>

class Helper
{
public:
    Helper();
    ~Helper();
    
    static float        randFloat(float from, float to);
    static int          randInt(int from, int to);

    template<typename... Args>
    static std::string formatMessage(const char* format, Args... args)
    {
        std::string buffer;
        buffer.resize(512);
        int size = snprintf(const_cast<char*>(buffer.c_str()), 512, format, args...);

        ASSERT(size >= 0, "The formated message should correctly be copied in the buffer");

        buffer.resize(size);
        return buffer;
    }
};
