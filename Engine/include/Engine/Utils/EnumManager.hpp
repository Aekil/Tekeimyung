/**
* @Author   Guillaume Labey
*/

#pragma once

template <typename T>
struct EnumManager
{};

// Trick to expend __VA_ARGS_
// (Thanks Buggy VC++)
#define EXPEND(x) x

#define GENERATE_STRING_1(STRING) #STRING,
#define GENERATE_STRING_2(STRING, VALUE) #STRING,

#define GENERATE_ENUM_1(ENUM) ENUM,
#define GENERATE_ENUM_2(ENUM, VALUE) ENUM = VALUE,

#define GENERATE_CONDITION_STRING_1(ENUM)             \
    if (enum_ == TmpEnum::ENUM)                     \
        return #ENUM;
#define GENERATE_CONDITION_STRING_2(ENUM, VALUE)    \
    if (enum_ == TmpEnum::ENUM)                     \
        return #ENUM;

#define GENERATE_CONDITION_ENUM_1(ENUM)               \
    if (enumString == #ENUM)                        \
        return TmpEnum::ENUM;
#define GENERATE_CONDITION_ENUM_2(ENUM, VALUE)      \
    if (enumString == #ENUM)                        \
        return TmpEnum::ENUM;

#define CHOOSE_FUNC(arg1, arg2, ...) arg2
#define CHOOSE_FUNC2(arg1, arg2, arg3, ...) arg3
#define CHOOSE_FUNC3(arg1, arg2, arg3, arg4, ...) arg4

#define GENERATE_ENUM(...) EXPEND(CHOOSE_FUNC2(__VA_ARGS__,\
                                            GENERATE_ENUM_2,\
                                            GENERATE_ENUM_1,\
                                            )(__VA_ARGS__))
#define GENERATE_STRING(...) EXPEND(CHOOSE_FUNC2(__VA_ARGS__,\
                                            GENERATE_STRING_2,\
                                            GENERATE_STRING_1,\
                                            )(__VA_ARGS__))
#define GENERATE_CONDITION_STRING(...) EXPEND(CHOOSE_FUNC2(__VA_ARGS__,\
                                                GENERATE_CONDITION_STRING_2,\
                                                GENERATE_CONDITION_STRING_1,\
                                                )(__VA_ARGS__))
#define GENERATE_CONDITION_ENUM(...) EXPEND(CHOOSE_FUNC2(__VA_ARGS__,\
                                                GENERATE_CONDITION_ENUM_2,\
                                                GENERATE_CONDITION_ENUM_1,\
                                                )(__VA_ARGS__))

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

// If REGISTER_ENUM_MANAGER is used, DECLARE_ENUM_MANAGER used be defined in a .cpp file
// MSVC does not need the declaration in a .cpp file
// and the declaration "constexpr const char* EnumManager<ENUM_NAME>::enumStrings[];" does not work for VC++ 2015

#if defined(_MSC_VER)

    #define DECLARE_ENUM_MANAGER(ENUM_NAME)

#else

#define DECLARE_ENUM_MANAGER(ENUM_NAME)                                             \
constexpr const char* EnumManager<ENUM_NAME>::enumStrings[];                        \
constexpr uint32_t EnumManager<ENUM_NAME>::enumLength;

#endif

#define REGISTER_ENUM(ENUM_NAME, ENUM_TYPE, ENUM_LIST)                                      \
enum class ENUM_NAME: ENUM_TYPE                                                             \
{                                                                                           \
    ENUM_LIST(GENERATE_ENUM)                                                                \
};
