#pragma once

#include <exception>
#include <string>
#include <list>

class EngineException: public std::exception
{
public:
    template<typename ...Msg>
    EngineException(Msg ...msg)
    {
        std::list<std::string> msgList = {msg...};
        for (auto &&msg: msgList)
        {
            _what.append(msg);
        }
    }
    ~EngineException() throw() {}

    virtual const char* what() const throw()
    {
        return (_what.c_str());
    }

private:
    std::string         _what;
};