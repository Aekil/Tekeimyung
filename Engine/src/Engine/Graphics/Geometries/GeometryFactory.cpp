/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Graphics/Geometries/Circle.hpp>
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
        RessourceManager::getInstance()->registerResource<Geometry>(std::move(plane), "plane");
    }

    // Box
    {
        Box::sInfo boxInfos = {SIZE_UNIT, SIZE_UNIT, SIZE_UNIT};
        std::unique_ptr<Geometry> box = std::make_unique<Box>(boxInfos);
        RessourceManager::getInstance()->registerResource<Geometry>(std::move(box), "box");
    }

    // Sphere
    {
        Sphere::sInfo sphereInfos = {SIZE_UNIT};
        std::unique_ptr<Geometry> sphere = std::make_unique<Sphere>(sphereInfos);
        RessourceManager::getInstance()->registerResource<Geometry>(std::move(sphere), "sphere");
    }

    // Circle
    {
        Circle::sInfo circleInfos = {SIZE_UNIT, SIZE_UNIT};
        std::unique_ptr<Geometry> circle = std::make_unique<Circle>(circleInfos);
        RessourceManager::getInstance()->registerResource<Geometry>(std::move(circle), "circle");
    }
}

Geometry*   GeometryFactory::getGeometry(Geometry::eType type)
{
    Geometry* geometry;
    if (type == Geometry::eType::PLANE)
    {
        geometry = RessourceManager::getInstance()->getOrLoadResource<Geometry>("plane");
    }
    else if (type == Geometry::eType::BOX)
    {
        geometry = RessourceManager::getInstance()->getOrLoadResource<Geometry>("box");
    }
    else if (type == Geometry::eType::SPHERE)
    {
        geometry = RessourceManager::getInstance()->getOrLoadResource<Geometry>("sphere");
    }
    else if (type == Geometry::eType::CIRCLE)
    {
        geometry = RessourceManager::getInstance()->getOrLoadResource<Geometry>("circle");
    }
    else
    {
        EXCEPT(InvalidParametersException, "Unknown model type for sRenderComponent");
    }

    ASSERT(geometry != nullptr, "The geometry should be registered");

    return (geometry);
}
