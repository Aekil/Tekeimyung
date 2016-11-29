#pragma once

#include <map>
#include <functional>
#include <memory>

#include <Engine/Utils/Logger.hpp>

class Resolutions
{
public:
    Resolutions();
    ~Resolutions() { };

    std::function<void(int)> getResolutionByName(std::string);
    static std::shared_ptr<Resolutions> getInstance();
    static void onCollisionEnterPlayer(int entityId);

private:
    static std::shared_ptr<Resolutions> _instance;
    std::map<std::string, std::function<void(int entityId)> > _resolutionsMap;
};
