/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Material.hpp>
#include <Engine/Debug/DebugWindow.hpp>
#include <Engine/Debug/IInspectorImplementation.hpp>
#include <Engine/Utils/JsonValue.hpp>

#define MATERIALS_DIRECTORY    "resources/materials/"

class MaterialDebugWindow : public DebugWindow, public IInspectorImplementation
{
public:
    MaterialDebugWindow();
    MaterialDebugWindow(const ImVec2& pos, const ImVec2& size);
    ~MaterialDebugWindow();

    void        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    //  IInspectorImplementation.hpp
    void        populateInspector() override final;

    GENERATE_ID(MaterialDebugWindow);

private:
    void        displayMaterialsList();
    void        displayMaterialsProperties();

    bool        createMaterial(std::string name);
    bool        cloneMaterial(Material* material, std::string cloneName);
    void        saveMaterials();
    void        saveMaterial(Material* material, JsonValue& json);

private:
    Material*   _selectedMaterial{nullptr};
    const char* _selectedMaterialName{nullptr};
};
