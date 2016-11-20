#pragma once

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
};
