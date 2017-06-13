/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

class Font;

class Text
{
public:
    Text();
    ~Text();

    Text(const Text& text);
    Text& operator=(const Text& text);

    const Font*         getFont() const;
    const std::string&  getContent() const;
    uint32_t            getFontSize() const;
    const glm::vec4&    getColor() const;
    const glm::vec2&    getSize();

    void                setFont(Font* font);
    void                setContent(const std::string& content);
    void                setFontSize(uint32_t fontSize);
    void                setColor(const glm::vec4& color);

    void                addContent(const std::string& content);

    void                isDirty(bool dirty);
    bool                isDirty() const;

private:
    void                needUpdate();

private:
    Font*               _font{nullptr};
    std::string         _content;
    uint32_t            _fontSize{10};
    glm::vec4           _color{1.0f};

    glm::vec2           _size;
    bool _needUpdateSize{true}; // True if _size need  to be recalculated

    bool                _dirty{true}; // True if any changes (Used externally for alignment update)
};
