#pragma once

class Timer
{
public:
    Timer();
<<<<<<< HEAD
    virtual ~Timer() {}
    
    void                reset();
    double              getElapsedTime() const;
=======
    ~Timer();

    void            reset();
    double         	getElapsedTime() const;
>>>>>>> 283ac34c1adff2944fe0e528fd81d98014c88b39

private:
	// Time since last reset
	// getElapsedTime() return the elapsed time since last timer reset
    double              _lastReset;
};
