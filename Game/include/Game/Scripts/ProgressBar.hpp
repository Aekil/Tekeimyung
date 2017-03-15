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
    virtual ~ProgressBar();

    void init(const std::string& entityProgressBarEmpty, const std::string& entityProgressBar);
    void update();

protected:
    float _maxProgress;
    float _currentProgress;

protected:
    Entity* _progressBarEmpty{nullptr};
    sTransformComponent* _progressBarEmptyTransform{nullptr};

    Entity* _progressBar{nullptr};
    sTransformComponent* _progressBarTransform{nullptr};
};
