/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Material.hpp>
#include <Engine/Utils/DebugWindow.hpp>
#include <Engine/Utils/JsonValue.hpp>

#define MATERIALS_DIRECTORY    "resources/materials/"

class MaterialDebugWindow: public DebugWindow
{
public:
    MaterialDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    ~MaterialDebugWindow();

    void        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(MaterialDebugWindow);

private:
    void        displayMaterialsList();
    void        displayMaterialsProperties();

    bool        createMaterial(std::string name);
    bool        cloneMaterial(Material* material, std::string cloneName);
    void        saveMaterials();
    void        saveMaterial(Material* material, JsonValue& json);

private:
    Material* _selectedMaterial{nullptr};
    const char* _selectedMaterialName{nullptr};
};
