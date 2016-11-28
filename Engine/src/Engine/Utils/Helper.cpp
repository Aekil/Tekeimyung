#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <Engine/Utils/Debug.hpp>

#include <Engine/Utils/Helper.hpp>

Helper::Helper()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
}

Helper::~Helper() {}

float   Helper::randFloat(float from, float to)
{
    float max = from > to ? from : to;
    float min = from < to ? from : to;

    if (max - min == 0)
        return (0.0f);

    return (min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))));
}

int Helper::randInt(int from, int to)
{
    int max = from > to ? from : to;
    int min = from < to ? from : to;

    if (max - min == 0)
        return (0);

    return (min + (rand() / (RAND_MAX / (max - min))));
}

void    Helper::copyAssimpMat(const aiMatrix4x4& from, glm::mat4& to)
{
    to[0][0] = from.a1; to[1][0] = from.a2;
    to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2;
    to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2;
    to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2;
    to[2][3] = from.d3; to[3][3] = from.d4;
}

void    Helper::copyAssimpQuat(const aiQuaternion& from, glm::quat& to)
{
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
    to.w = from.w;
}

void    Helper::copyAssimpVec3(const aiVector3D& from, glm::vec3& to)
{
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
}

std::string Helper::lowerCaseString(const std::string& str)
{
    std::string lowerCase = str;
    std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);

    return (lowerCase);
}
