template <typename T>
ParamAnimation<T>::ParamAnimation(T* param): _param(param), _currentKeyFrame(0) {}

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
    if (_keyFrames.size() == 0)
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
    }

    return (true);
}

template <typename T>
void    ParamAnimation<T>::reset()
{
    _timer.reset();
    _currentKeyFrame = 0;
}

template <typename T>
T   ParamAnimation<T>::getNewValue(const sKeyFrame& keyFrame)
{
    float ellapsedTime = _timer.getElapsedTime();

    switch (keyFrame.easing)
    {
        case ParamAnimation<T>::eEasing::NONE:
            return Helper::lerp<T>(keyFrame.startValue, keyFrame.endValue, ellapsedTime / keyFrame.duration);
        case ParamAnimation<T>::eEasing::EASE_IN:
            return Helper::lerp<T>(keyFrame.startValue, keyFrame.endValue, sin(ellapsedTime / keyFrame.duration * glm::pi<float>() * 0.5f));
        case ParamAnimation<T>::eEasing::EASE_OUT:
            return Helper::lerp<T>(keyFrame.startValue, keyFrame.endValue, 1.0f - cos(ellapsedTime / keyFrame.duration * glm::pi<float>() * 0.5f));
        default:
        return (keyFrame.startValue);
    }
}
