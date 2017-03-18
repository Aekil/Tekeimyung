/**
* @Author   Guillaume Labey
*/

template <typename T>
ParamAnimation<T>::ParamAnimation(const std::string& name, T* param, eInterpolationType type):
                                IParamAnimation(name), _currentKeyFrame(0), _type(type), _elapsedTime(0.0f), _lastKeyTime(0.0f), _lastFrameId(0)
{
    setParam(param);
}

template <typename T>
ParamAnimation<T>::~ParamAnimation() {}

template <typename T>
bool    ParamAnimation<T>::compareKeyFramesByTime(const sKeyFrame& a, const sKeyFrame& b)
{
    return (a.time < b.time);
}

template <typename T>
void    ParamAnimation<T>::addKeyFrame(const sKeyFrame& keyFrame, bool sort)
{
    sKeyFrame key = keyFrame;
    key.id = _lastFrameId;
    _keyFrames.push_back(key);
    _lastFrameId++;

    if (sort)
        sortKeyFrames();
}

template <typename T>
void    ParamAnimation<T>::sortKeyFrames()
{
    std::sort(_keyFrames.begin(), _keyFrames.end(), compareKeyFramesByTime);
}

template <typename T>
bool    ParamAnimation<T>::update(float elapsedTime)
{
    _elapsedTime += elapsedTime;

    // There is no key frame in the param animation
    if (!_param || _keyFrames.size() == 0)
        return (false);

    // This is the end of the param animation
    if (_currentKeyFrame >= _keyFrames.size())
        return (false);

    // Update the param value
    const sKeyFrame& keyFrame = _keyFrames[_currentKeyFrame];
    T newValue = getNewValue(keyFrame);
    *_param += (newValue - _lastValue);
    _lastValue = newValue;

    // Go to next key frame if current is finished
    if (_elapsedTime >= keyFrame.time)
    {
        _lastKeyTime = keyFrame.time;
        _currentKeyFrame++;
        _startValue = keyFrame.value;
    }

    return (true);
}

template <typename T>
void    ParamAnimation<T>::reset()
{
    if (!_param)
        return;

    _elapsedTime = 0.0f;
    _lastKeyTime = 0.0f;
    _currentKeyFrame = 0;
    _startValue = _type == eInterpolationType::ABSOLUTE ? _initialValue : T();
}

template <typename T>
std::shared_ptr<IParamAnimation>    ParamAnimation<T>::clone()
{
    auto paramAnimation = std::make_shared<ParamAnimation<T>>(_name, nullptr, _type);

    for (const auto& keyFrame: _keyFrames)
    {
        paramAnimation->addKeyFrame(keyFrame, false);
    }

    return (paramAnimation);
}

template <typename T>
T   ParamAnimation<T>::getNewValue(const sKeyFrame& keyFrame)
{
    if (!keyFrame.time || !(keyFrame.time - _lastKeyTime))
        return (keyFrame.value);

    // Value between 0 and 1
    float time = (_elapsedTime - _lastKeyTime) / (keyFrame.time - _lastKeyTime);

    switch (keyFrame.easing)
    {
        case IParamAnimation::eEasing::NONE:
            return Helper::lerp<T>(_startValue, keyFrame.value, time);
        case IParamAnimation::eEasing::EASE_IN:
            return Helper::lerp<T>(_startValue, keyFrame.value, sin(time * glm::pi<float>() * 0.5f));
        case IParamAnimation::eEasing::EASE_OUT:
            return Helper::lerp<T>(_startValue, keyFrame.value, 1.0f - cos(time * glm::pi<float>() * 0.5f));
        case IParamAnimation::eEasing::EASE_IN_OUT:
            return Helper::lerp<T>(_startValue, keyFrame.value, Helper::smootherStep(time));
        case IParamAnimation::eEasing::EXPONENTIAL:
            return Helper::lerp<T>(_startValue, keyFrame.value, time * time);
        default:
            return (_startValue);
    }
}
