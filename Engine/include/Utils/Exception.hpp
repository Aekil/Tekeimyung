#pragma once

#include <exception>
#include <cstdint>
#include <string>
#include <cstdio>

#include "Utils/Debug.hpp"

#define FORMAT_BUFFER_SIZE  512

#if defined(_MSC_VER)
    #define FUNCTION __FUNCSIG__
#else
    #define FUNCTION __PRETTY_FUNCTION__
#endif


class Exception: public std::exception {
public:
    Exception(const char *typeName, const std::string &description, const char* file, const char* function, uint32_t line);
    Exception(Exception&& other) = default;
    Exception(const Exception& other) = default;
    Exception& operator=(const Exception& other) = default;
    Exception& operator=(Exception&& other) = default;
    ~Exception() throw() {}

    const std::string& getTypeName() const;
    const std::string& getDescription() const;
    const std::string& getFile() const;
    const std::string& getFunction() const;
    const long getLine() const;

    const char* what() const noexcept override;

private:
    std::string _typeName;
    std::string _description;
    std::string _file;
    std::string _function;
    uint32_t _line;

    mutable std::string _fullDesc;
};


class NotImplementedException: public Exception {
public:
    NotImplementedException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"NotImplementedException", description, file, function, line} {}
};


class FileNotFoundException: public Exception {
public:
    FileNotFoundException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"FileNotFoundException", description, file, function, line} {}
};


class IOException: public Exception {
public:
    IOException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"IOException", description, file, function, line} {}
};


class InvalidParametersException: public Exception {
public:
    InvalidParametersException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"InvalidParametersException", description, file, function, line} {}
};


class InternalErrorException: public Exception {
public:
    InternalErrorException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"InternalErrorException", description, file, function, line} {}
};


class RendererAPIException: public Exception {
public:
    RendererAPIException(const std::string &description, const char* file, const char* function, uint32_t line)
    : Exception{"RendererAPIException", description, file, function, line} {}
};

template<typename... Args>
std::string formatMessage(const char* format, Args... args)
{
    std::string buffer;
    buffer.resize(FORMAT_BUFFER_SIZE);
    int size = snprintf(const_cast<char*>(buffer.c_str()), FORMAT_BUFFER_SIZE, format, args...);

    ASSERT(size >= 0, "The formated message should correctly be copied in the buffer");

    buffer.resize(size);
    return buffer;
}

#define EXCEPT(type, format, ...)\
do {\
    throw type(formatMessage(format, ## __VA_ARGS__), __FILE__, FUNCTION, __LINE__);\
} while (0)
