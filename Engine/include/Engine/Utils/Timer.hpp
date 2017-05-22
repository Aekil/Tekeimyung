/**
* @Author   Guillaume Labey
*/

#pragma once

#define SEC_TO_MS(s) ((s) * 1000)

class Timer
{
public:
    Timer();
    ~Timer();

    void                reset();
    float               getElapsedTime() const;

private:
	// Time since last reset
	// getElapsedTime() return the elapsed time since last timer reset
    float               _lastReset;
};
