/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <map>

#include <Engine/Core/BaseScript.hpp>

class ScriptFactory
{
public:
    // creates an object from a string
    static std::unique_ptr<BaseScript> create(const std::string& id)
    {
        const Creators_t::const_iterator iter = static_creators().find(id);

        // if found, execute the creator function pointer
        if (iter == static_creators().end())
            return (nullptr);
        auto script = (*iter->second)();
        script->setName(id);

        return (std::move(script));
    }

    typedef std::unique_ptr<BaseScript> Creator_t(); // function pointer to create Object
    typedef std::map<std::string, Creator_t*> Creators_t; // map from id to creator

    static Creators_t& static_creators()
    {
        static Creators_t s_creators;

        return (s_creators);
    } // static instance of map

    template<class T = int> class Register
    {
    public:
        static std::unique_ptr<BaseScript> create()
        {
            return (std::make_unique<T>());
        };

        static Creator_t* init_creator(const char* id)
        {
            getScriptsNames().push_back(id);
            return (static_creators()[id] = create);
        }

        static Creator_t* creator;
    };

    static std::vector<const char*>& getScriptsNames()
    {
        static std::vector<const char*> creatorsNames;

        return creatorsNames;
    }

};

#define REGISTER_SCRIPT(T) template<> ScriptFactory::Creator_t* ScriptFactory::Register<T>::creator = ScriptFactory::Register<T>::init_creator(#T)
