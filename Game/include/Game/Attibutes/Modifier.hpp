/*
** @Author : Simon AMBROISE
*/

#pragma once

class Modifier
{
private:
    double _value;

public:
    Modifier(double);
    ~Modifier();
    double getValue() const;

    inline bool operator==(const Modifier& rhs) const
    {
        return rhs.getValue() == this->getValue();
    }
};