/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/JsonValue.hpp>
#include <Engine/Core/GameState.hpp>

#define REGISTER_GAMESTATE(NAME)                                    \
    LevelLoader::getInstance()->addLoadedState<NAME##Factory>();


class LevelLoader
{
 public:
    LevelLoader();
    ~LevelLoader();

    static LevelLoader*                 getInstance();

    const std::vector<const char*>&     getLevels() const;
    std::vector<const char*>&           getLevels();
    void                                addLevel(const std::string& levelName);
    bool                                hasLevel(const std::string& levelName);

    void                                save(const std::string& levelName, const std::vector<Entity*>& entities);
    void                                load(const std::string& levelName, EntityManager* em);
    void                                loadDirectory(const std::string& directory);

    template <typename T>
    void                                addLoadedState()
    {
        if (T::identifier == EditorState::identifier)
        {
            ASSERT(true, "The EditorState can't be registered");
        }
        _loadedStates.push_back(std::make_unique<T>());
    }

    std::shared_ptr<GameState>          createLevelState(const std::string& levelName, GameStateManager* gameStateManager);

private:
    void                                addComponentToEntity(Entity* entity, const std::string& entityTypeName, const JsonValue& componentJson, const std::string& componentName);

private:
    std::vector<const char*>            _levels;
    std::vector<std::unique_ptr<GameStateFactory> > _loadedStates;

    static std::unique_ptr<LevelLoader> _instance;
};
