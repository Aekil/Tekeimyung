/**
* @Author   Mathieu Chassara
*/

#include <Engine/Debug/InspectorDebugWindow.hpp>

#include <Engine/Debug/LevelEntitiesDebugWindow.hpp>
#include <Engine/Debug/MaterialDebugWindow.hpp>

InspectorDebugWindow::InspectorDebugWindow() : DebugWindow("Inspector")
{

}

InspectorDebugWindow::~InspectorDebugWindow() {}

void    InspectorDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed))
    {
        ImGui::End();
        return;
    }

    if (this->_populateFuncPtr != nullptr)
        this->_populateFuncPtr();

    ImGui::End();
}

template<typename T>
void    InspectorDebugWindow::bindPopulateFunction(std::shared_ptr<T> debugWindow)
{
    this->_populateFuncPtr = std::bind(&T::populateInspector, debugWindow);
}

template void   InspectorDebugWindow::bindPopulateFunction(std::shared_ptr<LevelEntitiesDebugWindow> debugWindow);
template void   InspectorDebugWindow::bindPopulateFunction(std::shared_ptr<MaterialDebugWindow> debugWindow);