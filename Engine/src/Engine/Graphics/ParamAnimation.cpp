/**
* @Author   Guillaume Labey
*/

#include <Engine/Graphics/ParamAnimation.hpp>

std::vector<const char*>  IParamAnimation::_easingTypesString = { EASING_TYPES(GENERATE_EASING_STRING) };

std::unordered_map<std::string, IParamAnimation::eEasing>  IParamAnimation::_easingTypesFromString = { EASING_TYPES(GENERATE_EASING_FROM_STRING_PAIRS) };

std::unordered_map<IParamAnimation::eEasing, std::string>  IParamAnimation::_easingTypesFromEnum = { EASING_TYPES(GENERATE_EASING_FROM_ENUM_PAIRS) };
