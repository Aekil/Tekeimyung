/*
** @Author : Simon AMBROISE
*/

#pragma once

struct sTransformComponent;
struct sRenderComponent;
class Entity;

class ProgressBar
{
public:
    ProgressBar() = default;
    ~ProgressBar();

    void init(const std::string& entityProgressBarEmpty, const std::string& entityProgressBar);
    void update();
    void display(bool display);
    bool isDisplayed();

public:
    float maxProgress;
    float currentProgress;

    Entity* empty{nullptr};
    sTransformComponent* emptyTransform{nullptr};
    sRenderComponent*   emptyRender{nullptr};

    Entity* filled{nullptr};
    sTransformComponent* filledTransform{nullptr};
    sRenderComponent*   filledRender{nullptr};
};
