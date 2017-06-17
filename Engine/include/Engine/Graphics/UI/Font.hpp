/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

# include <Engine/Graphics/Texture.hpp>
# include <Engine/Utils/Resource.hpp>

#define CHECK_FT_ERROR(error, ...)                  \
    if (error)                                      \
    {                                               \
        LOG_ERROR(__VA_ARGS__);                     \
        return (false);                             \
    }
class Font: public Resource
{
private:
    struct sChar
    {
        Texture texture;
        glm::ivec2 bearing;
        int advance = 0;
    };

public:
    Font();
    ~Font();

    bool                        loadFromFile(const std::string &fileName) override;

    const sChar*                getChar(char c) const;
    const std::string&          getName() const;
    uint32_t                    getLetterSpacing() const;
    static Resource::eType      getResourceType() { return Resource::eType::FONT; }

private:
    FT_Face                     _face;
    bool                        _loaded{false};
    std::vector<sChar>          _chars;
};
