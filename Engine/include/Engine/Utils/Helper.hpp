#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

class Helper
{
public:
    Helper();
    ~Helper();

    static float        randFloat(float from, float to);
    static int          randInt(int from, int to);

    static void         copyAssimpMat(const aiMatrix4x4& from, glm::mat4& to);
    static void         copyAssimpQuat(const aiQuaternion& from, glm::quat& to);
    static void         copyAssimpVec3(const aiVector3D& from, glm::vec3& to);
    static std::string  lowerCaseString(const std::string& str);

    template <typename... Args>
    static std::string  formatMessage(const char* format, Args... args)
    {
        std::string buffer;
        buffer.resize(512);
        int size = snprintf(const_cast<char*>(buffer.c_str()), 512, format, args...);

        buffer.resize(size);
        return buffer;
    }

    template <typename T>
    static T            lerp(T from, T to, float time)
    {
        return ((1.0f - time) * from + time * to);
    }

    static float        parametricBlend(float t);
    static float        smoothStep(float t);
    static float        smootherStep(float t);
};
