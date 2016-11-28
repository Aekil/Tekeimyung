#pragma once

#include <memory>

template<typename... Args>
std::string formatMessage(const char* format, Args... args)
{
    std::string buffer;
    buffer.resize(512);
    int size = snprintf(const_cast<char*>(buffer.c_str()), 512, format, args...);

    ASSERT(size >= 0, "The formated message should correctly be copied in the buffer");

    buffer.resize(size);
    return buffer;
}