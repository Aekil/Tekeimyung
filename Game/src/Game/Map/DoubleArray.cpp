#include <cstring>

#include <ECS/Entity.hpp>

#include <Engine/Debug/Debug.hpp>

#include <Game/Map/DoubleArray.hpp>
#include <Game/Scripts/Path.hpp>

template <typename T>
DoubleArray<T>::sLayer::sLayer(DoubleArray<T>* array, int x): _array(array), _x(x) {}

template <typename T>
T&    DoubleArray<T>::sLayer::operator[](int y)
{
    return _array->_content[_x * _array->_height + y];
}

template <typename T>
DoubleArray<T>::~DoubleArray()
{
    if (_content)
    {
        delete[] _content;
    }
}

template <typename T>
uint32_t    DoubleArray<T>::getWidth() const
{
    return (_width);
}

template <typename T>
uint32_t    DoubleArray<T>::getHeight() const
{
    return (_height);
}

template <typename T>
bool    DoubleArray<T>::isAllocated() const
{
    return (_allocated);
}

template <typename T>
typename DoubleArray<T>::sLayer  DoubleArray<T>::operator[](int x)
{
    return DoubleArray<T>::sLayer(this, x);
}

template <typename T>
void    DoubleArray<T>::allocate(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;
    _content = new T[_width * _height];
    _allocated = true;
    clear();
}

template <typename T>
void    DoubleArray<T>::free()
{
    _width = 0;
    _height = 0;
    _content = nullptr;
    _allocated = false;
    delete[] _content;
}

template <typename T>
void    DoubleArray<T>::clear()
{
    if (!_content)
    {
        return;
    }
    std::memset(_content, 0, sizeof(T) * (_width * _height));
}

template int&  DoubleArray<int>::sLayer::operator[](int y);
template DoubleArray<int>::~DoubleArray();
template uint32_t  DoubleArray<int>::getWidth() const;
template uint32_t  DoubleArray<int>::getHeight() const;
template bool DoubleArray<int>::isAllocated() const;
template DoubleArray<int>::sLayer  DoubleArray<int>::operator[](int y);
template void  DoubleArray<int>::allocate(uint32_t width, uint32_t height);
template void  DoubleArray<int>::free();
template void  DoubleArray<int>::clear();


template char&  DoubleArray<char>::sLayer::operator[](int y);
template DoubleArray<char>::~DoubleArray();
template uint32_t  DoubleArray<char>::getWidth() const;
template uint32_t  DoubleArray<char>::getHeight() const;
template bool DoubleArray<char>::isAllocated() const;
template DoubleArray<char>::sLayer  DoubleArray<char>::operator[](int y);
template void  DoubleArray<char>::allocate(uint32_t width, uint32_t height);
template void  DoubleArray<char>::free();
template void  DoubleArray<char>::clear();


template Entity*&  DoubleArray<Entity*>::sLayer::operator[](int y);
template DoubleArray<Entity*>::~DoubleArray();
template uint32_t  DoubleArray<Entity*>::getWidth() const;
template uint32_t  DoubleArray<Entity*>::getHeight() const;
template bool DoubleArray<Entity*>::isAllocated() const;
template DoubleArray<Entity*>::sLayer  DoubleArray<Entity*>::operator[](int y);
template void  DoubleArray<Entity*>::allocate(uint32_t width, uint32_t height);
template void  DoubleArray<Entity*>::free();
template void  DoubleArray<Entity*>::clear();


template Node*&  DoubleArray<Node*>::sLayer::operator[](int y);
template DoubleArray<Node*>::~DoubleArray();
template uint32_t  DoubleArray<Node*>::getWidth() const;
template uint32_t  DoubleArray<Node*>::getHeight() const;
template bool DoubleArray<Node*>::isAllocated() const;
template DoubleArray<Node*>::sLayer  DoubleArray<Node*>::operator[](int y);
template void  DoubleArray<Node*>::allocate(uint32_t width, uint32_t height);
template void  DoubleArray<Node*>::free();
template void  DoubleArray<Node*>::clear();
