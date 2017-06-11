/**
* @Author   Guillaume Labey
*/

#include <sstream>

#include <Engine/Core/Components/Components.hh>
#include <Engine/Core/Components/ComponentsFactories.hh>
#include <Engine/Utils/Exception.hpp>

#include <Engine/Core/Components/IComponentFactory.hpp>

std::unordered_map<std::string, IComponentFactory*>  IComponentFactory::_componentsTypes = { COMPONENTS_TYPES(GENERATE_PAIRS) };
std::unordered_map<uint32_t, std::string>  IComponentFactory::_componentsTypesHashs = { COMPONENTS_TYPES(GENERATE_PAIRS_HASHS) };

bool    IComponentFactory::componentTypeExists(const std::string& type)
{
    for (auto &&componentType: _componentsTypes)
    {
        if (componentType.first == type)
            return (true);
    }

    return (false);
}

void    IComponentFactory::initComponent(const std::string& entityType, const std::string& name, const JsonValue& value)
{
    try
    {
        sComponent* component;
        component = _componentsTypes[name]->loadFromJson(entityType, JsonValue(value));
        _componentsTypes[name]->addComponent(entityType, component);
        _componentsTypes[name]->saveComponentJson(entityType, value.get());
    }
    catch(const std::exception& e)
    {
        std::stringstream msg;

        msg << "Failed to load " << name << ": " << e.what();
        EXCEPT(InternalErrorException, msg.str().c_str());
    }
}

sComponent*  IComponentFactory::createComponent(const std::string& entityType, const std::string& name)
{
    return _componentsTypes[name]->clone(entityType);
}

IComponentFactory*   IComponentFactory::getFactory(const std::string& name)
{
    if (!IComponentFactory::componentTypeExists(name))
        EXCEPT(InvalidParametersException, " IComponentFactory::getFactory: Component factory \"%s\" does not exist", name.c_str());

    return _componentsTypes[name];
}

std::string IComponentFactory::getComponentNameWithHash(uint32_t hash)
{
    return (_componentsTypesHashs[hash]);
}

std::size_t IComponentFactory::getComponentHashWithName(const std::string& name)
{
    for (auto component: _componentsTypesHashs)
    {
        if (component.second == name)
            return (component.first);
    }
    return (-1);
}

const std::unordered_map<uint32_t, std::string>& IComponentFactory::getComponentsTypesHashs()
{
    return (_componentsTypesHashs);
}
