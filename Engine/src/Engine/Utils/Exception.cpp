/**
* @Author   Guillaume Labey
*/

#include <sstream>

#include <Engine/Debug/Logger.hpp>

#include <Engine/Utils/Exception.hpp>

Exception::Exception(const char *typeName, const std::string &description, const char* file, const char* function, uint32_t line)
    : _typeName{typeName}, _description{description}, _file{file}, _function{function}, _line{line}
    {
        LOG_ERROR("Exception thrown: %s", what());
    }

const std::string& Exception::getTypeName() const {
    return _typeName;
}

const std::string& Exception::getDescription() const {
    return _description;
}

const std::string& Exception::getFile() const {
    return _file;
}

const std::string& Exception::getFunction() const {
    return _function;
}

const long Exception::getLine() const {
    return _line;
}

const char* Exception::what() const noexcept {
    std::stringstream msg;

    msg << _typeName << ": " << _description << std::endl;
    msg << "In " << _file;
    msg << " at " << _function << " line " << _line;

    _fullDesc = msg.str();
    return _fullDesc.c_str();
}
