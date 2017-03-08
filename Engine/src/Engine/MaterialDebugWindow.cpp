/**
* @Author   Guillaume Labey
*/

#include <cstdio>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/MaterialDebugWindow.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>

MaterialDebugWindow::MaterialDebugWindow(const glm::vec2& pos, const glm::vec2& size): DebugWindow("Material editor", pos, size) {}

MaterialDebugWindow::~MaterialDebugWindow() {}

void    MaterialDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    if (ImGui::Button("Create"))
    {
        ImGui::OpenPopup("create material");
    }
    else if (ImGui::Button("Save all"))
    {
        saveMaterials();
    }

    if (ImGui::BeginPopup("create material"))
    {
        static char newTypeName[64] = "";

        ImGui::InputText("New material", newTypeName, 64);
        if (ImGui::Button("Create"))
        {
            if (createMaterial(newTypeName))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    displayMaterialsList();
    displayMaterialsProperties();

    ImGui::End();
}

void    MaterialDebugWindow::displayMaterialsList()
{
    ImGui::BeginChild("Materials list", ImVec2(_size.x * 40.0f / 100.0f, 0), true);
    auto& materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();
    for (const char* materialName: materialNames)
    {
        Material* material = ResourceManager::getInstance()->getResource<Material>(materialName);
        if (!material->isModelMaterial() && ImGui::Selectable(materialName, _selectedMaterialName == materialName))
        {
            _selectedMaterialName = materialName;
            _selectedMaterial = material;
        }
    }
    ImGui::EndChild();
}

void    MaterialDebugWindow::displayMaterialsProperties()
{
    ImGui::SameLine();
    ImGui::BeginChild("Properties", ImVec2(_size.x * 60.0f / 100.0f, 0), false);

    if (_selectedMaterial != nullptr)
    {
        Texture* ambientTexture = _selectedMaterial->getTexture(Texture::eType::AMBIENT);
        Texture* diffuseTexture = _selectedMaterial->getTexture(Texture::eType::DIFFUSE);
        std::string ambientTextureName = ambientTexture ? ambientTexture->getId() : "";
        std::string diffuseTextureName = diffuseTexture ? diffuseTexture->getId() : "";

        // Ambient
        {
            glm::vec4 ambient = _selectedMaterial->getAmbient();
            if (ImGui::ColorEdit4("Ambient", glm::value_ptr(ambient)))
            {
                _selectedMaterial->setAmbient(ambient);
            }
        }

        // Diffuse
        {
            glm::vec4 diffuse = _selectedMaterial->getDiffuse();
            if (ImGui::ColorEdit4("Diffuse", glm::value_ptr(diffuse)))
            {
                _selectedMaterial->setDiffuse(diffuse);
            }
        }

        if (Helper::updateComboString("Ambient texture", ResourceManager::getInstance()->getResourcesNames<Texture>(), ambientTextureName))
        {
            // Empty choice
            if (ambientTextureName.size() == 0)
                _selectedMaterial->setTexture(Texture::eType::AMBIENT, nullptr);
            else
            {
                ambientTexture = ResourceManager::getInstance()->getResource<Texture>(ambientTextureName);
                _selectedMaterial->setTexture(Texture::eType::AMBIENT, ambientTexture);
            }
        }
        if (Helper::updateComboString("Diffuse texture", ResourceManager::getInstance()->getResourcesNames<Texture>(), diffuseTextureName))
        {
            // Empty choice
            if (diffuseTextureName.size() == 0)
                _selectedMaterial->setTexture(Texture::eType::DIFFUSE, nullptr);
            else
            {
                diffuseTexture = ResourceManager::getInstance()->getResource<Texture>(diffuseTextureName);
                _selectedMaterial->setTexture(Texture::eType::DIFFUSE, diffuseTexture);
            }
        }

        bool faceCamera = _selectedMaterial->isFacingCamera() ? true : false;
        if (ImGui::Checkbox("Face camera", &faceCamera))
        {
            _selectedMaterial->isFacingCamera(faceCamera == true ? 1 : 0);
        }
        ImGui::Checkbox("Transparent", &_selectedMaterial->transparent);
        ImGui::Checkbox("Wireframe", &_selectedMaterial->wireframe);

        // Blending modes
        if (_selectedMaterial->transparent)
        {
            // Src blend
            std::string srcBlendString = Material::getBlendStringFromEnum(_selectedMaterial->srcBlend);
            if (Helper::updateComboString("Src blend", Material::getBlendModes(), srcBlendString))
            {
                _selectedMaterial->srcBlend = Material::getBlendEnumFromString(srcBlendString);
            }

            // Dst blend
            std::string dstBlendString = Material::getBlendStringFromEnum(_selectedMaterial->dstBlend);
            if (Helper::updateComboString("Dst blend", Material::getBlendModes(), dstBlendString))
            {
                _selectedMaterial->dstBlend = Material::getBlendEnumFromString(dstBlendString);
            }
        }

        if (ImGui::Button("Clone"))
        {
            ImGui::OpenPopup("clone material");
        }

        if (ImGui::BeginPopup("clone material"))
        {
            static char newTypeName[64] = "";

            ImGui::InputText("New material", newTypeName, 64);
            if (ImGui::Button("Clone"))
            {
                if (cloneMaterial(_selectedMaterial, newTypeName))
                {
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::EndChild();
}

bool    MaterialDebugWindow::createMaterial(std::string name)
{
    name = name + ".mat";
    auto& materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();
    if (std::find(materialNames.cbegin(), materialNames.cend(), name) != materialNames.cend())
    {
        LOG_ERROR("Can't create material \"%s\": already exists", name.c_str());
        return (false);
    }

    std::string filePath = ResourceManager::getInstance()->getDirectoryPath("materials") + name;
    std::unique_ptr<Material> material = std::make_unique<Material>(false);
    ResourceManager::getInstance()->registerResource<Material>(std::move(material), filePath);
    LOG_INFO("New material \"%s\" has been created", name.c_str());

    return (true);
}

bool    MaterialDebugWindow::cloneMaterial(Material* material, std::string cloneName)
{
    if (!material)
    {
        LOG_ERROR("Can't create material clone \"%s\": base material does not exists", cloneName.c_str());
        return (false);
    }

    cloneName = cloneName + ".mat";
    auto& materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();
    if (std::find(materialNames.cbegin(), materialNames.cend(), cloneName) != materialNames.cend())
    {
        LOG_ERROR("Can't clone material \"%s\" with clone name \"%s\": already exists", material->getId().c_str(), cloneName.c_str());
        return (false);
    }

    std::string filePath = ResourceManager::getInstance()->getDirectoryPath("materials") + cloneName;
    std::unique_ptr<Material> cloneMaterial = std::make_unique<Material>(*material);
    ResourceManager::getInstance()->registerResource<Material>(std::move(cloneMaterial), filePath);
    LOG_INFO("Material \"%s\" has been cloned to \"%s\"", material->getId().c_str(), cloneName.c_str());

    return (true);
}

void    MaterialDebugWindow::saveMaterials()
{
    auto& materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();
    for (const char* materialName: materialNames)
    {
        Material* material = ResourceManager::getInstance()->getResource<Material>(materialName);

        // Only save non-model materials
        if (!material->isModelMaterial())
        {
            JsonWriter jsonWriter;
            JsonValue json;
            JsonValue texturesJson;

            json.setColor4f("ambient", material->getAmbient());
            json.setColor4f("diffuse", material->getDiffuse());
            json.setBool("face_camera", material->isFacingCamera() ? true : false);
            json.setBool("transparent", material->transparent);
            json.setBool("wireframe", material->wireframe);
            json.setString("src_blend", Material::getBlendStringFromEnum(material->srcBlend));
            json.setString("dst_blend", Material::getBlendStringFromEnum(material->dstBlend));

            if (material->getTexture(Texture::eType::AMBIENT))
            {
                texturesJson.setString("ambient", material->getTexture(Texture::eType::AMBIENT)->getPath());
            }
            else
            {
                texturesJson.setString("ambient", "");
            }
            if (material->getTexture(Texture::eType::DIFFUSE))
            {
                texturesJson.setString("diffuse", material->getTexture(Texture::eType::DIFFUSE)->getPath());
            }
            else
            {
                texturesJson.setString("diffuse", "");
            }

            json.setValue("textures", texturesJson);
            saveMaterial(material, json);
        }
    }
    LOG_INFO("All materials has been saved");
}

void    MaterialDebugWindow::saveMaterial(Material* material, JsonValue& json)
{
    JsonWriter jsonWriter;

    jsonWriter.write(material->getPath(), json);

    // The material name has changed, rename the file
    // TODO: Handle rename with material path (not recreating the full path, or it will move the engine materials in the game materials)
/*    if (material->getPath().size() != 0 && material->getPath() != filePath)
    {
        if (std::rename(material->getPath().c_str(), filePath.c_str()))
        {
            EXCEPT(InternalErrorException, "Failed to rename material file from \"%s\" to \"%s\"", material->getPath().c_str(), filePath.c_str());
        }
    }


    material->setPath(filePath);*/
}
