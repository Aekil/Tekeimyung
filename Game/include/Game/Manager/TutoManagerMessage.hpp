/**
* @Author   Julien CHARDON
*/

#pragma once

#include <memory>

#include <Game/Manager/TutoManager.hpp>

class TutoManagerMessage
{
friend TutoManager;

public:
    TutoManagerMessage() = default;
    ~TutoManagerMessage() = default;

    static TutoManagerMessage*                  getInstance();

    void                                        sendMessage(eTutoState state);
    bool                                        stateOnGoingOrDone(eTutoState state);
    bool                                        stateOnGoing(eTutoState state);
    bool                                        tutorialDone();

private:
    void                                        init();

private:
    static std::unique_ptr<TutoManagerMessage>  _instance;

    TutoManager*                                _tutoManager = nullptr;
};
