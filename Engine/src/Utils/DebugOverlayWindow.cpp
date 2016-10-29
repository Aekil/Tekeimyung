#include <imgui.h>

#include <Utils/DebugOverlayWindow.hpp>

DebugOverlayWindow::DebugOverlayWindow(): DebugWindow() {}

DebugOverlayWindow::~DebugOverlayWindow() {}

void	DebugOverlayWindow::build()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin(_title.c_str(), &_displayed, ImVec2(0, 0), 0.3f,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Average: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();
	ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::End();
}
