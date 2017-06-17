/**
* @Author   Guillaume Labey
*/

#include <algorithm>
#include <iostream>

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/NameComponent.hh>
#include <Engine/Core/Components/RenderComponent.hh>
#include <Engine/Core/Components/TransformComponent.hh>
#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/File.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/EntityFactory.hpp>

#include <dirent.h> // include dirent at the end or it could conflict with other includes

std::unordered_map<std::string, EntityFactory::sEntityInfo >  EntityFactory::_entities;
std::unordered_map<std::string, std::string>  EntityFactory::_entitiesFiles;
std::vector<const char*>  EntityFactory::_typesString;
EntityManager*  EntityFactory::_em = nullptr;

EntityFactory::EntityFactory() {}

EntityFactory::~EntityFactory()
{
    for (const char* typeString: _typesString)
    {
        delete typeString;
    }
}

void EntityFactory::loadDirectory(const std::string& archetypesDir)
{
    DIR* dir;
    struct dirent* ent;

    dir = opendir(archetypesDir.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory \"%s\"", archetypesDir.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        if (ResourceManager::getFileExtension(ent->d_name) == "json")
        {
            // Get entity configuration file
            std::string path = std::string(archetypesDir).append("/").append(ent->d_name);

            // Parse the configuration file with jsoncpp
            JsonReader jsonReader;
            JsonValue parsed;
            std::string typeName;
            std::string tag;
            if (!jsonReader.parse(path, parsed))
                EXCEPT(IOException, "Cannot parse archetype \"%s\"", path.c_str());

            typeName = parsed.getString("name", "");
            tag = parsed.getString("tag", "");
            LOG_INFO("Load entity %s", typeName.c_str());

            if (EntityFactory::entityTypeExists(typeName)) // The macro ENTITIES_TYPES did not create the type
                EXCEPT(InvalidParametersException, "Failed to read entity archetype \"%s\": Entity type \"%s\" already exist", path.c_str(), typeName.c_str());

            _entitiesFiles[typeName] = path;
             _entities[typeName].tag = tag;
             _typesString.push_back(strdup(typeName.c_str()));

            // Create entity components
            auto &&components = parsed.get("components", {}).get();
            for (Json::ValueIterator it = components.begin(); it != components.end(); it++)
            {
                std::string componentName = it.key().asString();

                // Component does not exists
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                {
                    // TODO: Use LOG_ERROR (Why is it not working ?)
                    LOG_WARN("EntityFactory::loadDirectory: Component type \"%s\" does not exist when loading entity \"%s\"", componentName.c_str(), typeName.c_str());
                    continue;
                }

                LOG_INFO("Add %s component %s", typeName.c_str(), componentName.c_str());
                IComponentFactory::initComponent(typeName, componentName, JsonValue(*it));
                _entities[typeName].components.push_back(componentName);
            }

            if (std::find(_entities[typeName].components.begin(), _entities[typeName].components.end(), "sNameComponent") == _entities[typeName].components.end())
                EXCEPT(InvalidParametersException, "Failed to read entity archetype \"%s\": missing sNameComponent", typeName.c_str());

            // Add sTransformation component if not found
            if (std::find(_entities[typeName].components.begin(), _entities[typeName].components.end(), "sTransformComponent") == _entities[typeName].components.end())
            {
                IComponentFactory::initComponent(typeName, "sTransformComponent", {});
                _entities[typeName].components.push_back("sTransformComponent");
            }
        }
        // No file extension, is directory
        else if (std::string(ent->d_name).find(".") == std::string::npos)
        {
            // Load directory
            std::string directoryPath = std::string(archetypesDir).append("/").append(ent->d_name);
            loadDirectory(directoryPath);
        }
    }

    closedir(dir);
}

bool    EntityFactory::entityTypeExists(const std::string& type)
{
    for (auto &&entityType : _typesString)
    {
        if (entityType == type)
            return (true);
    }

    return (false);
}

Entity* EntityFactory::createOrGetEntity(const std::string& typeName)
{
    for (Entity* entity : _em->getEntities())
    {
        sNameComponent* name = entity->getComponent<sNameComponent>();

        ASSERT(name != nullptr, "The entity should have a sNameComponent");

        if (name->value == typeName)
        {
            return (entity);
        }
    }

    return (createEntity(typeName));
}

Entity* EntityFactory::createEntity(const std::string& typeName, const glm::vec3& pos, bool store)
{
    Entity *entity = cloneEntity(typeName, store);

    sTransformComponent* transform = entity->getComponent<sTransformComponent>();

    transform->setPos(pos);

    return (entity);
}

Entity* EntityFactory::createEntity(const std::string& typeName, bool store)
{
    return (cloneEntity(typeName, store));
}

void EntityFactory::createEntityType(const std::string& typeName)
{
    // Add entity to factory
    std::string filePath = ARCHETYPES_LOCATION + std::string("/") + Helper::lowerCaseString(typeName) + ".json";
    _entitiesFiles[typeName] = filePath;
    _typesString.push_back(_entitiesFiles.find(typeName)->first.c_str());

    // Add transform component
    IComponentFactory::initComponent(typeName, "sTransformComponent", {});
    _entities[typeName].components.push_back("sTransformComponent");

    // Add name component
    JsonValue nameComponentJson;
    nameComponentJson.setString("name", typeName);
    IComponentFactory::initComponent(typeName, "sNameComponent", nameComponentJson);
    _entities[typeName].components.push_back("sNameComponent");

    // Check if file does not exist
    {
        File* file = ResourceManager::getInstance()->getResource<File>(filePath);
        if (file)
        {
            // TODO: Replace with LOG_ERROR (Why it does not work ??)
            LOG_WARN("Can't create entity type \"%s\": the file \"%s\" already exists", typeName.c_str(), filePath.c_str());
            return;
        }
    }

    // Register and save file
    {
        std::unique_ptr<File> resourceFile = std::make_unique<File>();

        File* file = ResourceManager::getInstance()->registerResource<File>(std::move(resourceFile), filePath);
        if (file)
        {
            JsonValue entityJson;
            JsonValue components;
            components.setValue("sNameComponent", nameComponentJson);
            entityJson.setString("name", typeName);
            entityJson.setValue("components", components);

            file->setContent(entityJson.get().toStyledString());
            file->save();
        }
    }

    LOG_INFO("New entity type \"%s\" created", typeName.c_str());

}

void EntityFactory::bindEntityManager(EntityManager* em)
{
    _em = em;
}

EntityManager*  EntityFactory::getBindedEntityManager()
{
    return _em;
}

const std::vector<const char*>& EntityFactory::getTypesString()
{
    return _typesString;
}

const EntityFactory::sEntityInfo&   EntityFactory::getInfos(const std::string& typeName)
{
    return _entities[typeName];
}

const void   EntityFactory::removeComponent(const std::string& typeName, const std::string& component)
{
    auto& components = _entities[typeName].components;
    auto foundComponent = std::find(components.begin(), components.end(), component);
    if (foundComponent != components.end())
    {
        components.erase(foundComponent);
    }
}

const void   EntityFactory::addComponent(const std::string& typeName, const std::string& component)
{
    _entities[typeName].components.push_back(component);
}

const void   EntityFactory::setTag(const std::string& typeName, const std::string& tag)
{
    _entities[typeName].tag = tag;
}

const std::string& EntityFactory::getFile(const std::string& typeName)
{
    return _entitiesFiles[typeName];
}

Entity* EntityFactory::cloneEntity(const std::string& typeName, bool store)
{
    Entity* clone = _em->createEntity(store);
    auto entity = _entities.find(typeName);

    if (entity == _entities.end())
        EXCEPT(InvalidParametersException, "The entity type %s does not exist", typeName.c_str());

    clone->setTag(entity->second.tag);

    for (auto &&component : entity->second.components)
    {
        sComponent* component_ = IComponentFactory::createComponent(typeName, component);
        clone->addComponent(component_);
    }

    initAnimations(clone);
    _em->notifyEntityCreated(clone);

    return (clone);
}

void    EntityFactory::copyEntityManager(EntityManager* dst, EntityManager* src)
{
    auto& entities = src->getEntities();
    for (Entity* entity: entities)
    {
        Entity* cloneEntity = dst->createEntity();

        cloneEntity->setTag(entity->getTag());

        auto& components = entity->getComponents();
        for (auto& component : components)
        {
            sComponent* cloneComponent = component->clone();
            cloneEntity->addComponent(cloneComponent);
        }

        initAnimations(cloneEntity);
        _em->notifyEntityCreated(cloneEntity);
    }
}

void    EntityFactory::initAnimations(Entity* entity)
{
    sRenderComponent* render = entity->getComponent<sRenderComponent>();

    if (!render || render->_animator.getAnimationsNb() == 0)
        return;

    sTransformComponent* transform = entity->getComponent<sTransformComponent>();

    for (auto animation : render->_animator.getAnimations())
    {
        for (auto paramAnimation : animation->getParamsAnimations())
        {
            if (transform && paramAnimation->getName() == "position")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->_pos);
            else if (transform && paramAnimation->getName() == "rotation")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->_rotation);
            else if (transform && paramAnimation->getName() == "scale")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->_scale);
            else if (transform && paramAnimation->getName() == "color")
                std::static_pointer_cast<ParamAnimation<glm::vec4>>(paramAnimation)->setParam(&render->color);
        }
    }
}

void    EntityFactory::reverseAnimations(Entity* entity)
{
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    if (render && render->_animator.isPlaying())
    {
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        render->_animator.reset();
        transform->getTransform();
    }
}

void    EntityFactory::updateEntitiesComponents(Entity* from, const std::string& entityName, IComponentFactory* compFactory, sComponent* component)
{
    for (Entity* entity: _em->getEntities())
    {
        sNameComponent* name = entity->getComponent<sNameComponent>();

        if (name->value == entityName && entity != from)
        {
            sComponent* entityComponent = entity->getComponent(component->id);

            // Add component to entity
            if (!entityComponent)
            {
                entity->addComponent(component->clone());
            }
            // Update component
            else
            {
                // Only the scale have to be copied
                if (component->id == sTransformComponent::identifier)
                {
                    sTransformComponent* transform = static_cast<sTransformComponent*>(component);
                    sTransformComponent* entityTransform = static_cast<sTransformComponent*>(entityComponent);

                    entityTransform->setScale(transform->getScale());
                    entityTransform->needUpdate();
                }
                else
                {
                    entityComponent->update(component);
                }

                if (component->id == sRenderComponent::identifier)
                {

                    initAnimations(entity);

                    sRenderComponent* render = static_cast<sRenderComponent*>(entityComponent);

                    if (render && render->_animator.getAnimationsNb() > 0)
                    {
                        AnimationPtr currentAnimation = render->_animator.getAnimations()[0];
                        render->_animator.play(currentAnimation->getName());
                    }
                }
            }
        }
    }
}

void    EntityFactory::saveEntityTemplateToJson(const std::string& typeName)
{
    JsonWriter jsonWriter;
    JsonValue json;
    JsonValue components;
    std::string savedFile = EntityFactory::getFile(typeName);
    auto&& entityInfos = EntityFactory::getInfos(typeName);

    json.setString("name", typeName);
    json.setString("tag", entityInfos.tag);
    for (auto &&component: entityInfos.components)
    {
        JsonValue& componentJson = IComponentFactory::getFactory(component)->saveToJson(typeName);
        components.setValue(component, componentJson);
    }
    json.setValue("components", components);

    jsonWriter.write(savedFile, json);

    LOG_INFO("Entity template %s saved to %s", typeName.c_str(), savedFile.c_str());
}

void    EntityFactory::saveEntityTemplate(const std::string& typeName, Entity* entity)
{
    EntityFactory::setTag(typeName, entity->getTag());

    {
        auto& components = EntityFactory::getInfos(typeName).components;
        // Remove deleted components from factories
        for (auto it = components.begin(); it != components.end();)
        {
            auto component = *it;
            std::size_t componentHash = IComponentFactory::getComponentHashWithName(component);
            auto compFactory = IComponentFactory::getFactory(component);
            ASSERT(compFactory != nullptr, "The factory should exist");

            // The component has been removed, delete it from EntityFactory and ComponentFactory
            if (!entity->hasComponent(componentHash))
            {
                ++it;
                EntityFactory::removeComponent(typeName, component);
                compFactory->remove(typeName);
            }
            else
                ++it;
        }
    }

    {
        auto& components = EntityFactory::getInfos(typeName).components;

        // Reverse animations
        EntityFactory::reverseAnimations(entity);

        // Save entity components
        for (auto component: entity->getComponents())
        {
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
            auto compFactory = IComponentFactory::getFactory(componentName);
            ASSERT(compFactory != nullptr, "The factory should exist");

            compFactory->save(typeName, component);

            // The component does not exist in entityFactory
            if (std::find(components.begin(), components.end(), componentName) == components.end())
            {
                // Add component to EntityFactory
                EntityFactory::addComponent(typeName, componentName);
            }

            // Update other entities component
            EntityFactory::updateEntitiesComponents(entity, typeName, compFactory, component);
        }
    }

    {
        // Remove entities components if entity does not has the component
        for (Entity* entity : _em->getEntities())
        {
            sNameComponent* name = entity->getComponent<sNameComponent>();

            if (name->value == typeName && entity != entity)
            {
                auto& components = entity->getComponents();
                auto it = components.begin();
                for (it; it != components.end();)
                {
                    sComponent* component = *it;
                    if (!entity->hasComponent(component->id))
                    {
                        delete component;
                        it = components.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }

    LOG_INFO("Entity template %s overwritten by entity", typeName.c_str());
}
