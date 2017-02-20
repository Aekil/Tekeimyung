/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Graphics/Geometries/Circle.hpp>
#include <Engine/Graphics/Geometries/Cone.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Geometries/GeometryFactory.hpp>

GeometryFactory::GeometryFactory() {}

GeometryFactory::~GeometryFactory() {}

void GeometryFactory::initGeometries()
{
    // Plane
    {
        Plane::sInfo planeInfos = {SIZE_UNIT, SIZE_UNIT};
        std::unique_ptr<Geometry> plane = std::make_unique<Plane>(planeInfos);
        ResourceManager::getInstance()->registerResource<Geometry>(std::move(plane), "plane");
    }

    // Box
    {
        Box::sInfo boxInfos = {SIZE_UNIT, SIZE_UNIT, SIZE_UNIT};
        std::unique_ptr<Geometry> box = std::make_unique<Box>(boxInfos);
        ResourceManager::getInstance()->registerResource<Geometry>(std::move(box), "box");
    }

    // Sphere
    {
        Sphere::sInfo sphereInfos = {SIZE_UNIT / 2.0f};
        std::unique_ptr<Geometry> sphere = std::make_unique<Sphere>(sphereInfos);
        ResourceManager::getInstance()->registerResource<Geometry>(std::move(sphere), "sphere");
    }

    // Circle
    {
        Circle::sInfo circleInfos = {SIZE_UNIT / 2.0f, SIZE_UNIT / 2.0f};
        std::unique_ptr<Geometry> circle = std::make_unique<Circle>(circleInfos);
        ResourceManager::getInstance()->registerResource<Geometry>(std::move(circle), "circle");
    }

    // Cone
    {
        Cone::sInfo coneInfos = {SIZE_UNIT / 2.0f, SIZE_UNIT};
        std::unique_ptr<Geometry> cone = std::make_unique<Cone>(coneInfos);
        ResourceManager::getInstance()->registerResource<Geometry>(std::move(cone), "cone");
    }
}

Geometry*   GeometryFactory::getGeometry(Geometry::eType type)
{
    Geometry* geometry;
    if (type == Geometry::eType::PLANE)
    {
        geometry = ResourceManager::getInstance()->getOrLoadResource<Geometry>("plane");
    }
    else if (type == Geometry::eType::BOX)
    {
        geometry = ResourceManager::getInstance()->getOrLoadResource<Geometry>("box");
    }
    else if (type == Geometry::eType::SPHERE)
    {
        geometry = ResourceManager::getInstance()->getOrLoadResource<Geometry>("sphere");
    }
    else if (type == Geometry::eType::CIRCLE)
    {
        geometry = ResourceManager::getInstance()->getOrLoadResource<Geometry>("circle");
    }
    else if (type == Geometry::eType::CONE)
    {
        geometry = ResourceManager::getInstance()->getOrLoadResource<Geometry>("cone");
    }
    else
    {
        EXCEPT(InvalidParametersException, "Unknown model type for sRenderComponent");
    }

    ASSERT(geometry != nullptr, "The geometry should be registered");

    return (geometry);
}
