/**
* @Author   Julien Chardon
*/

#include <cmath>
#include <iostream>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>

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
        _sounds[i].volume = DEFAULT_SOUND_VOL;
        _sounds[i].sound = nullptr;
        _sounds[i].channel = nullptr;
        _sounds[i].type = eSoundType::DEFAULT_SOUND;
    }
    /*for (int i = 0; i < NB_MAX_CHANNELS; ++i)
    {
        _channels[i] = nullptr;
    }*/
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
    if (errorCheck())
        return (false);

    _result = _system->init(NB_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (errorCheck())
        return (false);

    _result = _system->getMasterChannelGroup(&_channelGroup);
    if (errorCheck())
        return (false);

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
        LOG_ERROR("FMOD error! (%d) : %s", _result, FMOD_ErrorString(_result));
        //std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (true);
    }
    return (false);
}

void    SoundManager::freeSound(int id)
{
    if (_sounds[id].free == false)
    {
        _sounds[id].free = true;
        _result = _sounds[id].sound->release();
        errorCheck();
    }
    else
    {
        LOG_WARN("Sound with id n°%d is already released", id);
    }
}

int     SoundManager::registerSound(const std::string& name, eSoundType type)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        // The sound type changed, reload it to optimize
        /*if (_sounds[i].name == name &&
            _sounds[i].type != type &&
            _sounds[i].type != eSoundType::NONE)
        {
            _sounds[i].sound->release();
            _sounds[i].sound = nullptr;
            _sounds[i].free = true;
        }*/

        if (_sounds[i].free)
        {
            _sounds[i].free = false;
            _sounds[i].type = type;
            _sounds[i].name = name;
            _sounds[i].id = i;

            if (type == eSoundType::BACKGROUND_SOUND /*|| type == eSoundType::NONE*/)
            {
                //_sounds[i].type = eSoundType::BACKGROUND_SOUND;
                _result = _system->createStream(name.c_str(), FMOD_LOOP_NORMAL, 0, &_sounds[i].sound);
            }
            else // type == DEFAULT_SOUND
            {
                _result = _system->createSound(name.c_str(), FMOD_DEFAULT, 0, &_sounds[i].sound);
            }
            if (!errorCheck())
                LOG_INFO("sound : %s has been successfully loaded", name.c_str());
            else
                LOG_WARN("sound : %s has not been properly loaded", name.c_str());
            return (_sounds[i].id);
        }
        else if (_sounds[i].name == name)
        {
            LOG_INFO("sound : %s is already loaded", name.c_str());
            return (_sounds[i].id);
        }
    }
    LOG_WARN("No available space to register (& load) more sounds");
    return (-1);
}

void    SoundManager::playSound(int id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS) // Out of range
    {
        LOG_WARN("Sound id %d out of range in playSound(id)", id);
        return;
    }

    _result = _system->playSound(_sounds[id].sound, 0, false, &_sounds[id].channel);
    errorCheck();

    _result = _sounds[id].channel->setVolume(_sounds[id].volume);
    errorCheck();

    if (_sounds[id].channel == nullptr)
    {
        LOG_ERROR("Failed to play sound : channel handle is null");
        return;
    }

    //addChannel(_sounds[id].channel);
}

/*void    SoundManager::resumeSound(int id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    _result = _sounds[id].channel->setPaused(false);
    errorCheck();
}

void    SoundManager::pauseSound(int id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    _result = _sounds[id].channel->setPaused(true);
    errorCheck();
}

void    SoundManager::stopSound(int id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    removeChannel(_sounds[id].channel);
    errorCheck();

    _result = _sounds[id].channel->stop();
    errorCheck();

    _sounds[id].channel = nullptr;
}*/

bool    SoundManager::isSoundPlaying(int id)
{
    bool isPlaying = false;

    if (id < 0 || id >= NB_MAX_SOUNDS) // out of range
    {
        LOG_WARN("Sound id %d out of range in isSoundPlaying(id)", id);
        return (false);
    }
    if (_sounds[id].channel == nullptr || _sounds[id].free) // Not currently playing
    {
        return (false);
    }

    _result = _sounds[id].channel->isPlaying(&isPlaying);
    errorCheck();

    return (isPlaying);
}

/*bool    SoundManager::isSoundPaused(int id)
{
    bool isPaused = false;

    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return (false);

    _result = _sounds[id].channel->getPaused(&isPaused);
    errorCheck();

    return (isPaused);
}*/

/*unsigned int        SoundManager::getSoundCurrentPosition(int id)
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
}*/

int     SoundManager::getSoundIDFromName(const std::string& name) const
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (!_sounds[i].free && _sounds[i].name == name)
            return (_sounds[i].id);
    }
    LOG_WARN("Sound %s is not registered", name.c_str());
    return (-1);
}

const char*   SoundManager::getSoundNameFromID(int id) const
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (!_sounds[i].free && i == id)
            return (_sounds[i].name.c_str());
    }
    LOG_WARN("Sound with id n°%d is not registered", id);
    return (nullptr);
}

void    SoundManager::pauseAllChannels()
{
    bool chanGrpState;

    _result = _channelGroup->getPaused(&chanGrpState);
    errorCheck();

    if (!chanGrpState)
    {
        _result = _channelGroup->setPaused(1);
        errorCheck();
    }
}

void    SoundManager::resumeAllChannels()
{
    bool chanGrpState;
    
    _result = _channelGroup->getPaused(&chanGrpState);
    errorCheck();

    if (chanGrpState)
    {
        _result = _channelGroup->setPaused(0);
        errorCheck();
    }
}
// limits of volume ? (0.0f -> 1.0f ?)
void    SoundManager::setVolumeAllChannels(float volume)
{
    _result = _channelGroup->setVolume(volume);
    errorCheck();
}
// Differentiate (general) volume from all channels to personalized volume for one channel
void    SoundManager::setSoundVolume(int id, float volume)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (i == id)
        {
            if (!_sounds[i].free)
            {
                _sounds[i].volume = volume;
                /*_result = _sounds[i].channel->setVolume(_sounds[i].volume); // ?
                errorCheck();*/
            }
            else
            {
                LOG_WARN("Sound with id n°%d is not registered", id);
            }
            return;
        }
    }
    LOG_WARN("Sound id n°%d doesn't exist", id);
}

/*void    SoundManager::addChannel(FMOD::Channel* channel)
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
}*/

/*tSoundInfos         SoundManager::getSoundInfos(uint32_t soundID)
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
}*/
