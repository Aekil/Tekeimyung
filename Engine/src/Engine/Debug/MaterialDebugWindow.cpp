/**
* @Author   Guillaume Labey
*/

#include <cstdio>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonWriter.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/EntityFactory.hpp>

#include <Engine/Debug/MaterialDebugWindow.hpp>

MaterialDebugWindow::MaterialDebugWindow() : DebugWindow("Materials")
{
    this->_size.x = GameWindow::getInstance()->getBufferWidth() - 500;
    this->_size.y = GameWindow::getInstance()->getBufferHeight() - 300;
}

MaterialDebugWindow::MaterialDebugWindow(const ImVec2& pos, const ImVec2& size) :
    DebugWindow("Materials", pos, size) {}

MaterialDebugWindow::~MaterialDebugWindow() {}

void    MaterialDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Create")) ImGui::OpenPopup("create_material");
    ImGui::SameLine();
    if (this->_selectedMaterialName != nullptr && this->_selectedMaterial != nullptr && ImGui::Button("Save"))
        this->saveMaterial(this->_selectedMaterialName);
    ImGui::SameLine();
    if (ImGui::Button("Save all")) this->saveMaterials();
    ImGui::SameLine();

    if (ImGui::BeginPopup("create_material"))
    {
        static char newTypeName[64] = "";

        ImGui::InputText("Material name", newTypeName, 64);
        if (std::string(newTypeName).size() > 0)
        {
            ImGui::Separator();
            if (ImGui::Button("Confirm"))
            {
                if (createMaterial(newTypeName))
                {
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        ImGui::EndPopup();
    }

    this->displayMaterialsList();
    ImGui::SameLine();
    this->populateInspector();
    ImGui::End();
}

void    MaterialDebugWindow::populateInspector()
{
    if (this->_selectedMaterialName != nullptr &&
        this->_selectedMaterial != nullptr)
    {
        this->displayMaterialsProperties();
    }
}

void    MaterialDebugWindow::displayMaterialsList()
{
    static ImGuiTextFilter  filter;

    filter.Draw();
    ImGui::Separator();
    ImGui::BeginChild("Materials list", ImVec2(_size.x * 40.0f / 100.0f, 0), true);

    auto&   materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();

    for (const char* materialName: materialNames)
    {
        if (filter.PassFilter(materialName) == true)
        {
            Material*   material = ResourceManager::getInstance()->getResource<Material>(materialName);

            if (!material->isModelMaterial() && ImGui::Selectable(materialName, _selectedMaterialName == materialName))
            {
                _selectedMaterialName = materialName;
                _selectedMaterial = material;
            }
        }
    }

    ImGui::EndChild();
}

void    MaterialDebugWindow::displayMaterialsProperties()
{
    bool changed = false;
    ImGui::BeginChild("Properties", ImVec2(300, 0), false);

    ImGui::Text("Material name:\t%s", this->_selectedMaterialName);
    ImGui::Separator();

    if (_selectedMaterial != nullptr)
    {
        // Ambient
        {
            glm::vec4 ambient = _selectedMaterial->getAmbient();
            if (ImGui::ColorEdit4("Ambient", glm::value_ptr(ambient)))
            {
                _selectedMaterial->setAmbient(ambient);
                changed = true;
            }
        }

        // Diffuse
        {
            glm::vec4 diffuse = _selectedMaterial->getDiffuse();
            if (ImGui::ColorEdit4("Diffuse", glm::value_ptr(diffuse)))
            {
                _selectedMaterial->setDiffuse(diffuse);
                changed = true;
            }
        }

        // Bloom color
        {
            if (_selectedMaterial->hasBloom())
            {
                glm::vec4 bloom = _selectedMaterial->getBloom();
                if (ImGui::ColorEdit4("Bloom", glm::value_ptr(bloom)))
                {
                    _selectedMaterial->setBloom(bloom);
                    changed = true;
                }
            }
        }

        // Ambient texture
        {
            Texture* texture = _selectedMaterial->getTexture(Texture::eType::AMBIENT);
            std::string textureName = texture ? texture->getId() : "";

            if (Helper::updateComboString("Ambient", ResourceManager::getInstance()->getResourcesNames<Texture>(), textureName))
            {
                // Empty choice
                if (textureName.size() == 0)
                    _selectedMaterial->setTexture(Texture::eType::AMBIENT, nullptr);
                else
                {
                    texture = ResourceManager::getInstance()->getResource<Texture>(textureName);
                    _selectedMaterial->setTexture(Texture::eType::AMBIENT, texture);
                }
                changed = true;
            }
        }

        // Diffuse texture
        {
            Texture* texture = _selectedMaterial->getTexture(Texture::eType::DIFFUSE);
            std::string textureName = texture ? texture->getId() : "";

            if (Helper::updateComboString("Diffuse", ResourceManager::getInstance()->getResourcesNames<Texture>(), textureName))
            {
                // Empty choice
                if (textureName.size() == 0)
                    _selectedMaterial->setTexture(Texture::eType::DIFFUSE, nullptr);
                else
                {
                    texture = ResourceManager::getInstance()->getResource<Texture>(textureName);
                    _selectedMaterial->setTexture(Texture::eType::DIFFUSE, texture);
                }
                changed = true;
            }
        }

        // Bloom texture
        {
            if (_selectedMaterial->hasBloom())
            {
                Texture* texture = _selectedMaterial->getTexture(Texture::eType::BLOOM);
                std::string textureName = texture ? texture->getId() : "";

                if (Helper::updateComboString("Bloom", ResourceManager::getInstance()->getResourcesNames<Texture>(), textureName))
                {
                    // Empty choice
                    if (textureName.size() == 0)
                        _selectedMaterial->setTexture(Texture::eType::BLOOM, nullptr);
                    else
                    {
                        texture = ResourceManager::getInstance()->getResource<Texture>(textureName);
                        _selectedMaterial->setTexture(Texture::eType::BLOOM, texture);
                    }
                    changed = true;
                }
            }
        }

        // Bloom texture alpha
        {
            if (_selectedMaterial->hasBloom())
            {
                Texture* texture = _selectedMaterial->getTexture(Texture::eType::BLOOM_ALPHA);
                std::string textureName = texture ? texture->getId() : "";

                if (Helper::updateComboString("Bloom alpha", ResourceManager::getInstance()->getResourcesNames<Texture>(), textureName))
                {
                    // Empty choice
                    if (textureName.size() == 0)
                        _selectedMaterial->setTexture(Texture::eType::BLOOM_ALPHA, nullptr);
                    else
                    {
                        texture = ResourceManager::getInstance()->getResource<Texture>(textureName);
                        _selectedMaterial->setTexture(Texture::eType::BLOOM_ALPHA, texture);
                    }
                    changed = true;
                }
            }
        }

        bool faceCamera = _selectedMaterial->isFacingCamera();
        if (ImGui::Checkbox("Face camera", &faceCamera))
        {
            _selectedMaterial->isFacingCamera(faceCamera);
            changed = true;
        }
        bool bloom = _selectedMaterial->hasBloom();
        if (ImGui::Checkbox("Bloom", &bloom))
        {
            _selectedMaterial->hasBloom(bloom);
            changed = true;
        }
        changed |= ImGui::Checkbox("Transparent", &_selectedMaterial->transparent);
        changed |= ImGui::Checkbox("Wireframe", &_selectedMaterial->wireframe);

        // Blending modes
        if (_selectedMaterial->transparent)
        {
            // Src blend
            std::string srcBlendString = Material::getBlendStringFromEnum(_selectedMaterial->srcBlend);
            if (Helper::updateComboString("Src blend", Material::getBlendModes(), srcBlendString))
            {
                _selectedMaterial->srcBlend = Material::getBlendEnumFromString(srcBlendString);
                changed = true;
            }

            // Dst blend
            std::string dstBlendString = Material::getBlendStringFromEnum(_selectedMaterial->dstBlend);
            if (Helper::updateComboString("Dst blend", Material::getBlendModes(), dstBlendString))
            {
                _selectedMaterial->dstBlend = Material::getBlendEnumFromString(dstBlendString);
                changed = true;
            }
        }

        if (ImGui::Button("Clone"))
        {
            ImGui::OpenPopup("clone_material");
        }

        if (ImGui::BeginPopup("clone_material"))
        {
            static char newTypeName[64] = "";

            ImGui::InputText("New material", newTypeName, 64);
            if (ImGui::Button("OK !"))
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

    if (changed)
    {
        updateEntitiesMaterials(_selectedMaterial);
    }
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

    std::string                 filePath = std::string(MATERIALS_DIRECTORY) + name;
    std::unique_ptr<Material>   material = std::make_unique<Material>(false);

    ResourceManager::getInstance()->registerResource<Material>(std::move(material), filePath);

    //std::sort(materialNames.begin(), materialNames.end());
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

    auto&   materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();

    if (std::find(materialNames.cbegin(), materialNames.cend(), cloneName) != materialNames.cend())
    {
        LOG_ERROR("Can't clone material \"%s\" with clone name \"%s\": already exists", material->getId().c_str(), cloneName.c_str());
        return (false);
    }

    std::string                 filePath = std::string(MATERIALS_DIRECTORY) + cloneName;
    std::unique_ptr<Material>   cloneMaterial = std::make_unique<Material>(*material);

    ResourceManager::getInstance()->registerResource<Material>(std::move(cloneMaterial), filePath);
    //std::sort(materialNames.begin(), materialNames.end());
    LOG_INFO("Material \"%s\" has been cloned to \"%s\"", material->getId().c_str(), cloneName.c_str());

    return (true);
}

void    MaterialDebugWindow::saveMaterial(const char* materialName)
{
    Material*   material = ResourceManager::getInstance()->getResource<Material>(materialName);

    // Only save non-model materials
    if (!material->isModelMaterial())
    {
        JsonWriter  jsonWriter;
        JsonValue   json;
        JsonValue   texturesJson;

        json.setColor4f("ambient", material->getAmbient());
        json.setColor4f("diffuse", material->getDiffuse());
        json.setColor4f("bloom_color", material->getBloom());
        json.setBool("face_camera", material->isFacingCamera());
        json.setBool("has_bloom", material->hasBloom());
        json.setBool("transparent", material->transparent);
        json.setBool("wireframe", material->wireframe);
        json.setString("src_blend", Material::getBlendStringFromEnum(material->srcBlend));
        json.setString("dst_blend", Material::getBlendStringFromEnum(material->dstBlend));

        // Ambient texture
        if (material->getTexture(Texture::eType::AMBIENT))
        {
            texturesJson.setString("ambient", material->getTexture(Texture::eType::AMBIENT)->getPath());
        }
        else
        {
            texturesJson.setString("ambient", "");
        }

        // Diffuse texture
        if (material->getTexture(Texture::eType::DIFFUSE))
        {
            texturesJson.setString("diffuse", material->getTexture(Texture::eType::DIFFUSE)->getPath());
        }
        else
        {
            texturesJson.setString("diffuse", "");
        }

        // Bloom texture
        if (material->getTexture(Texture::eType::BLOOM))
        {
            texturesJson.setString("bloom", material->getTexture(Texture::eType::BLOOM)->getPath());
        }
        else
        {
            texturesJson.setString("bloom", "");
        }

        // Bloom texture alpha
        if (material->getTexture(Texture::eType::BLOOM_ALPHA))
        {
            texturesJson.setString("bloom_alpha", material->getTexture(Texture::eType::BLOOM_ALPHA)->getPath());
        }
        else
        {
            texturesJson.setString("bloom_alpha", "");
        }

        json.setValue("textures", texturesJson);
        this->saveMaterial(material, json);
    }
}

void    MaterialDebugWindow::saveMaterials()
{
    auto&   materialNames = ResourceManager::getInstance()->getResourcesNames<Material>();

    for (const char* materialName: materialNames)
        this->saveMaterial(materialName);

    LOG_INFO("All materials has been saved");
}

void    MaterialDebugWindow::saveMaterial(Material* material, JsonValue& json)
{
    JsonWriter  jsonWriter;
    std::string filePath = std::string(MATERIALS_DIRECTORY) + material->getId();

    jsonWriter.write(material->getPath(), json);

    // The material name has changed, rename the file
    if (material->getPath().size() != 0 && material->getPath() != filePath)
    {
        if (std::rename(material->getPath().c_str(), filePath.c_str()))
        {
            EXCEPT(InternalErrorException, "Failed to rename material file from \"%s\" to \"%s\"", material->getPath().c_str(), filePath.c_str());
        }
    }

    material->setPath(filePath);
}

void    MaterialDebugWindow::updateEntitiesMaterials(Material* material)
{
    auto em = EntityFactory::getBindedEntityManager();
    auto& entities = em->getEntities();

    for (Entity* entity: entities)
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (!render)
        {
            continue;
        }
        auto modelInstance = render->getModelInstance();
        auto& meshsInstances = modelInstance->getMeshsInstances();
        for (auto& meshInstance: meshsInstances)
        {
            if (meshInstance->getMaterial() &&
                meshInstance->getMaterial()->getId() == material->getId())
            {
                meshInstance->setMaterial(material);
            }
        }
    }
}
