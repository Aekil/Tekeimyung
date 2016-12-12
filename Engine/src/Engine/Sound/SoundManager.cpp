#include <cmath>
#include <iostream>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Sound/SoundManager.hpp>

std::shared_ptr<SoundManager>   SoundManager::_soundManager = nullptr;

SoundManager::SoundManager() :
_result(FMOD_OK),
_system(nullptr)
{
    for (int i = 0; i < NB_MAX_SOUNDS; ++i)
    {
        _sounds[i].free = true;
        _sounds[i].id = i;
        _sounds[i].sound = nullptr;
        _sounds[i].channel = nullptr;
        _sounds[i].type = eSoundType::DEFAULT_SOUND;
    }
    for (int i = 0; i < NB_MAX_CHANNELS; ++i)
    {
        _channels[i] = nullptr;
    }
}

SoundManager::~SoundManager() {}

std::shared_ptr<SoundManager>   SoundManager::getInstance()
{
    if (!_soundManager)
    {
        _soundManager = std::make_shared<SoundManager>();
    }
    return (_soundManager);
}

bool    SoundManager::initialize()
{
    _result = FMOD::System_Create(&_system);
    if (!errorCheck())
        return (false);

    _result = _system->init(NB_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (!errorCheck())
        return (false);

    //RessourceManager::getInstance()->loadResources(SOUNDS_DIRECTORY);

//#if (ENGINE_DEBUG) // let similar message in release ?
    //for (int i = 0; i < _soundsStrings.size(); ++i)
    //{
    //    LOG_INFO("Sound found : path = %s"/* | basename = %s"*/, _soundsStrings[i].name.c_str()/*, _soundsStrings[i].name.c_str()*/);
    //}
//#endif

    return (true);
}

void    SoundManager::update()
{
    _result = _system->update();
    errorCheck();
}

void    SoundManager::shutdown()
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
        freeSound(i);
    _result = _system->close();
    errorCheck();
    _result = _system->release();
    errorCheck();
}

bool    SoundManager::errorCheck()
{
    if (_result != FMOD_OK)
    {
        //LOG_ERROR("FMOD error! (%d) : %s", _result, FMOD_ErrorString(_result)); // hide bugs in logs
        //std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (false);
    }
    return (true);
}

void    SoundManager::freeSound(uint32_t id)
{
    if (_sounds[id].free == false)
    {
        _sounds[id].free = true;
        _result = _sounds[id].sound->release();
        errorCheck();
    }
}

int     SoundManager::registerSound(const std::string& name, eSoundType type)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        // The sound type changed, reload it to optimize
        if (_sounds[i].name == name &&
            _sounds[i].type != type &&
            _sounds[i].type != eSoundType::NONE)
        {
            _sounds[i].sound->release();
            _sounds[i].sound = nullptr;
            _sounds[i].free = true;
        }

        if (_sounds[i].free)
        {
            _sounds[i].free = false;
            _sounds[i].type = type;
            _sounds[i].name = name;
            _sounds[i].id = i;
            // Load music and default sound as streams
            if (type == eSoundType::BACKGROUND_SOUND || type == eSoundType::NONE)
            {
                _sounds[i].type = eSoundType::BACKGROUND_SOUND;
                _result = _system->createStream(name.c_str(), FMOD_LOOP_NORMAL, 0, &_sounds[i].sound);
            }
            else // type == DEFAULT_SOUND
            {
                _result = _system->createSound(name.c_str(), FMOD_DEFAULT, 0, &_sounds[i].sound);
            }
            LOG_INFO("Load sound : %s", name.c_str());
            errorCheck();
            return (_sounds[i].id);
        }
        else if (_sounds[i].name == name)
            return (_sounds[i].id);
    }
    LOG_INFO("No place available to register more sounds"); // log : info / warn ?
    return (-1); // no place available to register more sounds
}

void    SoundManager::playSound(uint32_t id)
{
    // Out of range
    if (id < 0 || id >= NB_MAX_SOUNDS)
        return;

    // Play sound
    _result = _system->playSound(_sounds[id].sound, 0, false, &_sounds[id].channel);
    errorCheck();

    // Add channel to array
    if (_sounds[id].channel == nullptr)
    {
        LOG_ERROR("Failed to play sound : channel handle is null");
        return;
    }

    addChannel(_sounds[id].channel);
}

void    SoundManager::resumeSound(uint32_t id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    _result = _sounds[id].channel->setPaused(false);
    errorCheck();
}

void    SoundManager::pauseSound(uint32_t id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    _result = _sounds[id].channel->setPaused(true);
    errorCheck();
}

void    SoundManager::stopSound(uint32_t id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    removeChannel(_sounds[id].channel);
    errorCheck();

    _result = _sounds[id].channel->stop();
    errorCheck();

    _sounds[id].channel = nullptr;
}

bool    SoundManager::isSoundPlaying(uint32_t id)
{
    bool isPlaying = false;

    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return (false);

    _result = _sounds[id].channel->isPlaying(&isPlaying);
    errorCheck();

    return (isPlaying);
}

bool    SoundManager::isSoundPaused(uint32_t id)
{
    bool isPaused = false;

    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return (false);

    _result = _sounds[id].channel->getPaused(&isPaused);
    errorCheck();

    return (isPaused);
}

unsigned int        SoundManager::getSoundCurrentPosition(int id)
{
    unsigned int    currentPosition;

    currentPosition = 0;
    if (id < 0 || id >= NB_MAX_SOUNDS ||    // Out of range
        _sounds[id].channel == nullptr)     // Not currently playing
        return (currentPosition);

    _result = _sounds[id].channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
    errorCheck();

    return (currentPosition);
}

unsigned int        SoundManager::getSoundLength(int id)
{
    tSound          sound;
    unsigned int    soundLength;

    soundLength = 0;
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (i == id)
        {
            if (!_sounds[i].free)
            {
                sound = _sounds[i];
                _result = sound.sound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
                errorCheck();
                return (soundLength);
            }
            else break;
        }
    }
    return (soundLength);
}

const char*   SoundManager::getSoundNameFromID(int id) const
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (i == id)
        {
            if (!_sounds[i].free)
            {
                return (_sounds[i].name.c_str());
            }
            else
            {
                break;
            }
        }
    }
    return (nullptr);
}

void    SoundManager::pause()
{
    for (int i = 0; i < NB_MAX_CHANNELS; ++i)
    {
        if (_channels[i] == nullptr)
            continue;

        _result = _channels[i]->setPaused(true);
        errorCheck();
    }
}

void    SoundManager::resume()
{
    for (int i = 0; i < NB_MAX_CHANNELS; ++i)
    {
        if (_channels[i] == nullptr)
            continue;

        _result = _channels[i]->setPaused(false);
        errorCheck();
    }
}

void    SoundManager::setVolume(float volume)
{
    for (int i = 0; i < NB_MAX_CHANNELS; ++i)
    {
        if (_channels[i] == nullptr)
            continue;

        _result = _channels[i]->setVolume(volume);
        errorCheck();
    }
}

void    SoundManager::addChannel(FMOD::Channel* channel)
{
    int index = 0;
    _result = channel->getIndex(&index);
    errorCheck();
    if (index < NB_MAX_CHANNELS &&
        index > 0)
    {
        _channels[index] = channel;
    }
    else
    {
        LOG_ERROR("SoundManager::addChannel: Channel index out of range");
    }
}

void    SoundManager::removeChannel(FMOD::Channel* channel)
{
    int index = 0;
    _result = channel->getIndex(&index);
    errorCheck();
    if (index < NB_MAX_CHANNELS &&
        index > 0)
    {
        _channels[index] = nullptr;
    }
    else
    {
        LOG_ERROR("SoundManager::removeChannel, Channel index out of range");
    }
}

tSoundInfos         SoundManager::getSoundInfos(uint32_t soundID)
{
    tSoundInfos     soundInfos;
    float           exponent, mantissa;
    float           exponent2, mantissa2;

    soundInfos.currentPosition = getSoundCurrentPosition(soundID);
    soundInfos.soundLength = getSoundLength(soundID);

    mantissa = std::modf(((float) soundInfos.currentPosition) / 1000.0f, &exponent);
    mantissa2 = std::modf(exponent / 60.0f, &exponent2);
    soundInfos.currentMinutes = (unsigned int) exponent2;
    soundInfos.currentSeconds = (unsigned int) (mantissa2 * 60.0f);

    mantissa = std::modf(((float) soundInfos.soundLength) / 1000.0f, &exponent);
    mantissa2 = std::modf(exponent / 60.0f, &exponent2);
    soundInfos.lengthMinutes = (unsigned int) exponent2;
    soundInfos.lengthSeconds = (unsigned int) (mantissa2 * 60.0f);

    return (soundInfos);
}