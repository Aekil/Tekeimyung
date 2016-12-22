#include <map>

#include <Engine/Core/BaseScript.hpp>

class ScriptFactory
{
public:
    // creates an object from a string
    static BaseScript* create(const std::string& id)
    {
        const Creators_t::const_iterator iter = static_creators().find(id);

        return (iter == static_creators().end() ? nullptr : (*iter->second)()); // if found, execute the creator function pointer
    }

    typedef BaseScript* Creator_t(); // function pointer to create Object
    typedef std::map<std::string, Creator_t*> Creators_t; // map from id to creator

    static Creators_t& static_creators()
    {
        static Creators_t s_creators;

        return (s_creators);
    } // static instance of map

    template<class T = int> class Register
    {
    public:
        static BaseScript* create()
        {
            return (new T());
        };

        static Creator_t* init_creator(const std::string& id)
        {
            return (static_creators()[id] = create);
        }

        static Creator_t* creator;
    };
};

#define REGISTER_SCRIPT(T, STR) template<> ScriptFactory::Creator_t* ScriptFactory::Register<T>::creator = ScriptFactory::Register<T>::init_creator(STR)