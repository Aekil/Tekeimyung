/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Geometries/Box.hpp>
#include <Engine/Graphics/Geometries/Plane.hpp>
#include <Engine/Graphics/Geometries/Sphere.hpp>
#include <Engine/Graphics/Geometries/Circle.hpp>

#include <Engine/Graphics/Geometries/GeometryFactory.hpp>

GeometryFactory::GeometryFactory() {}

GeometryFactory::~GeometryFactory() {}

std::shared_ptr<Geometry>   GeometryFactory::create(Geometry::eType& type, const std::string& texture)
{
    if (type == Geometry::eType::PLANE)
    {
        Plane::sInfo planeInfos = {1.0f, 1.0f};
        planeInfos.texturePath = texture;
        return std::make_shared<Plane>(planeInfos);
    }
    else if (type == Geometry::eType::BOX)
    {
        Box::sInfo boxInfos = {1.0f, 1.0f, 1.0f};
        return std::make_shared<Box>(boxInfos);
    }
    else if (type == Geometry::eType::SPHERE)
    {
        Sphere::sInfo sphereInfos = {1.0f};
        return std::make_shared<Sphere>(sphereInfos);
    }
    else if (type == Geometry::eType::CIRCLE)
    {
        Circle::sInfo circleInfos = {1.0f, 1.0f};
        return std::make_shared<Circle>(circleInfos);
    }
    else
    {
        EXCEPT(InvalidParametersException, "Unknown model type for sRenderComponent");
    }
}
