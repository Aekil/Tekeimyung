/*
** @Author : Simon AMBROISE
*/

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <Engine/Components.hh>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Game/Scripts/CameraScript.hpp>

void CameraScript::start()
{
    _cameraComp = entity->getComponent<sCameraComponent>();
    _scrollSpeed = 1500.0f;
}

void CameraScript::update(float dt)
{
    auto &gameWindow = GameWindow::getInstance();
    auto &mouse = gameWindow->getMouse();
    auto &keyboard = gameWindow->getKeyboard();
    auto& camera = _cameraComp->camera;

    // update Projection type
    {
        if (keyboard.isPressed(Keyboard::eKey::O))
            camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
        else if (keyboard.isPressed(Keyboard::eKey::P))
            camera.setProjType(Camera::eProj::PERSPECTIVE);
    }

    // update zoom
    {
        auto &&scroll = mouse.getScroll();
        static double lastScrollOffset = scroll.yOffset;


        double offset = scroll.yOffset - lastScrollOffset;

        if (offset)
        {
            camera.setProjSize(camera.getProjSize() + (float)offset * dt * _scrollSpeed);

            // Limit max projection size to 700
            camera.setProjSize(std::min(camera.getProjSize(), 700.0f));
            // Limit min projection size to 250
            camera.setProjSize(std::max(camera.getProjSize(), 250.0f));
            //camera.zoom((float)(-offset * dt));
        }
        lastScrollOffset = scroll.yOffset;
    }

    // update camera position when reaching edge
    {
        static float edgeDist = 80.0f;
        static float moveSpeed = 300.0f;
        auto& cursor = mouse.getCursor();
        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureMouse)
        {
            if (cursor.getX() > gameWindow->getBufferWidth() - edgeDist)
            {
                camera.translate({moveSpeed * dt, 0.0f, 0.0f}, Camera::eTransform::LOCAL);
            }
            if (cursor.getX() < edgeDist)
            {
                camera.translate({-moveSpeed * dt, 0.0f, 0.0f}, Camera::eTransform::LOCAL);
            }
            if (cursor.getY() > gameWindow->getBufferHeight() - edgeDist)
            {
                // Double moveSpeed for Y because it seems to be slower then X due to isometric view
                camera.translate({0.0f, -moveSpeed * dt, 0.0f}, Camera::eTransform::LOCAL);
            }
            if (cursor.getY() < edgeDist)
            {
                // Double moveSpeed for Y because it seems to be slower then X due to isometric view
                camera.translate({0.0f, moveSpeed * dt, 0.0f}, Camera::eTransform::LOCAL);
            }
        }

    }
}
