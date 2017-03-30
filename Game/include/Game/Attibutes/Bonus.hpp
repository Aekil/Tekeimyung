/*
** @Author : Simon AMBROISE
*/

#pragma once

class Bonus
{
private:
    double _value;

public:
    Bonus(double);
    ~Bonus();
    double getValue() const;

    inline bool operator==(const Bonus& rhs) const
    {
        return rhs.getValue() == this->getValue();
    }
};