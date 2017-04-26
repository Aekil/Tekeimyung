/*
** @Author : Simon AMBROISE
*/

#pragma once

class Bonus
{
private:
    int _value;

public:
    Bonus(int);
    ~Bonus();
    int getValue() const;

    inline bool operator==(const Bonus& rhs) const
    {
        return rhs.getValue() == this->getValue();
    }
};