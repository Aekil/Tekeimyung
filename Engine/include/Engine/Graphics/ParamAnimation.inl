template <typename T>
ParamAnimation<T>::ParamAnimation(const std::string& name, T* param): IParamAnimation(name), _param(param), _currentKeyFrame(0)
{
    if (_param)
    {
        _startValue = *_param;
        _initialValue = *_param;
    }
}

template <typename T>
ParamAnimation<T>::~ParamAnimation() {}

template <typename T>
void    ParamAnimation<T>::addKeyFrame(sKeyFrame& keyFrame)
{
    _keyFrames.push_back(keyFrame);
}

template <typename T>
bool    ParamAnimation<T>::update()
{
    // There is no key frame in the param animation
    if (!_param || _keyFrames.size() == 0)
        return (false);

    // This is the end of the param animation, reset it
    if (_currentKeyFrame >= _keyFrames.size())
        return (false);

    // Update the param value
    const sKeyFrame& keyFrame = _keyFrames[_currentKeyFrame];
    *_param = getNewValue(keyFrame);

    // Go to next key frame if current is finished
    if (_timer.getElapsedTime() >= keyFrame.duration)
    {
        _timer.reset();
        _currentKeyFrame++;
        _startValue = *_param;
    }

    return (true);
}

template <typename T>
void    ParamAnimation<T>::reset()
{
    if (!_param)
        return;

    _timer.reset();
    _currentKeyFrame = 0;
    _startValue = _initialValue;
}

template <typename T>
T   ParamAnimation<T>::getNewValue(const sKeyFrame& keyFrame)
{
    float ellapsedTime = _timer.getElapsedTime();

    switch (keyFrame.easing)
    {
        case ParamAnimation<T>::eEasing::NONE:
            return Helper::lerp<T>(_startValue, keyFrame.value, ellapsedTime / keyFrame.duration);
        case ParamAnimation<T>::eEasing::EASE_IN:
            return Helper::lerp<T>(_startValue, keyFrame.value, sin(ellapsedTime / keyFrame.duration * glm::pi<float>() * 0.5f));
        case ParamAnimation<T>::eEasing::EASE_OUT:
            return Helper::lerp<T>(_startValue, keyFrame.value, 1.0f - cos(ellapsedTime / keyFrame.duration * glm::pi<float>() * 0.5f));
        default:
        return (_startValue);
    }
}
