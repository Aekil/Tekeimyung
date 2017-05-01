#pragma once

#include <cstdint>

template<typename T>
class DoubleArray
{
public:
    struct sLayer
    {
    public:
        sLayer(DoubleArray* array, int x);

        T&    operator[](int y);

    private:
        DoubleArray* _array = nullptr;
        int _x = 0;
    };

friend sLayer;

public:
    DoubleArray() = default;
    virtual ~DoubleArray();

    uint32_t        getWidth() const;
    uint32_t        getHeight() const;
    bool            isAllocated() const;

    sLayer  operator[](int x);

    void    allocate(uint32_t width, uint32_t height);
    void    free();
    void    clear();

private:
    T*  _content = nullptr;
    bool _allocated = false;

protected:
    uint32_t _width = 0;
    uint32_t _height = 0;
};
