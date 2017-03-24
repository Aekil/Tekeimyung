#include <Engine/Graphics/UI/Text.hpp>

Text::Text() {}

Text::~Text() {}

Text::Text(const Text& text)
{
    _font = text._font;
    _content = text._content;
    _fontSize = text._fontSize;
    _color = text._color;
}

Text& Text::operator=(const Text& text)
{
    _font = text._font;
    _content = text._content;
    _fontSize = text._fontSize;
    _color = text._color;

    return (*this);
}

const Font* Text::getFont() const
{
    return (_font);
}

const std::string&  Text::getContent() const
{
    return (_content);
}

uint32_t    Text::getFontSize() const
{
    return (_fontSize);
}

const glm::vec4&    Text::getColor() const
{
    return (_color);
}

void    Text::setFont(Font* font)
{
    _font = font;
}

void    Text::setContent(const std::string& content)
{
    _content = content;
}

void    Text::setFontSize(uint32_t fontSize)
{
    _fontSize = fontSize;
}

void    Text::setColor(const glm::vec4& color)
{
    _color = color;
}
