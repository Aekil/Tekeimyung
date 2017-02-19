/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <imgui.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#define FMT_MSG(format, ...)    (Helper::formatMessage(format, ## __VA_ARGS__))


class Foo {
public:
 class monEnum {
    enum Test{
        VALUE0 = 0,
        VALUE1 = 1
    };
};
};


template <typename T>
struct EnumManager
{};


#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_ENUM(ENUM) ENUM,

#define GENERATE_CONDITION_STRING(ENUM)     \
    if (enum_ == TmpEnum::ENUM)             \
        return #ENUM;

#define GENERATE_CONDITION_ENUM(ENUM)       \
    if (enumString == #ENUM)                \
        return TmpEnum::ENUM;

#define REGISTER_ENUM_MANAGER(ENUM_NAME, ENUM_LIST)                                         \
template <>                                                                                 \
struct EnumManager<ENUM_NAME>                                                               \
{                                                                                           \
    static constexpr const char* enumStrings[] = {ENUM_LIST(GENERATE_STRING)};              \
    static constexpr uint32_t enumLength = sizeof(enumStrings) / sizeof(enumStrings[0]);    \
                                                                                            \
    static const char* enumToString(ENUM_NAME enum_)                                        \
    {                                                                                       \
          using TmpEnum = ENUM_NAME;                                                        \
                                                                                            \
          ENUM_LIST(GENERATE_CONDITION_STRING);                                             \
                                                                                            \
          return ("");                                                                      \
    }                                                                                       \
                                                                                            \
    static ENUM_NAME stringToEnum(const std::string& enumString)                            \
    {                                                                                       \
          using TmpEnum = ENUM_NAME;                                                        \
                                                                                            \
          ENUM_LIST(GENERATE_CONDITION_ENUM);                                               \
                                                                                            \
          return (ENUM_NAME());                                                             \
    }                                                                                       \
};

#define REGISTER_ENUM(ENUM_NAME, ENUM_TYPE, ENUM_LIST)                                      \
enum class ENUM_NAME: ENUM_TYPE                                                             \
{                                                                                           \
    ENUM_LIST(GENERATE_ENUM)                                                                \
};                                                                                          \

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

    static bool         updateComboString(const char* name, std::vector<const char*>& stringList, std::string& stringValue);
    template<typename T>
    static bool         updateComboEnum(const char* name, T& enum_)
    {
        int enumInt = (int)enum_;

        if (ImGui::Combo(name, &enumInt, (const char**)EnumManager<T>::enumStrings, (int)EnumManager<T>::enumLength))
        {
            enum_ = (T)enumInt;
            return (true);
        }
        return (false);
    }
};
