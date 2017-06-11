#include <Engine/Graphics/UI/Font.hpp>
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

const glm::vec2&    Text::getSize()
{
    if (_needUpdateSize)
    {
        _needUpdateSize = false;
        float fontScale = (float)_fontSize / 200.0f;
        float size_x = 0.0f;
        _size = {0.0f, _fontSize};

        for (uint32_t j = 0; j < _content.size(); ++j)
        {
            char c = _content[j];
            if (c == '\n')
            {
                _size.y += _fontSize;
                if (size_x > _size.x)
                    _size.x = size_x;
                size_x = 0.0f;
                continue;
            }

            auto char_ = _font->getChar(c);
            if (!char_)
                continue;


            size_x += (char_->advance >> 6) * fontScale;
        }

        if (size_x > _size.x)
            _size.x = size_x;
    }

    return (_size);
}

void    Text::setFont(Font* font)
{
    _font = font;
    needUpdate();
}

void    Text::setContent(const std::string& content)
{
    _content = content;
    needUpdate();
}

void    Text::setFontSize(uint32_t fontSize)
{
    _fontSize = fontSize;
    needUpdate();
}

void    Text::setColor(const glm::vec4& color)
{
    _color = color;
}

void    Text::addContent(const std::string& content)
{
    _content += content;
    needUpdate();
}

void    Text::isDirty(bool dirty)
{
    _dirty = dirty;
}

bool    Text::isDirty() const
{
    return (_dirty);
}

void    Text::needUpdate()
{
    _needUpdateSize = true;
    isDirty(true);
}
