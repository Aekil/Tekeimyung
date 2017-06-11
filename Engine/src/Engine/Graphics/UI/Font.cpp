/**
* @Author   Guillaume Labey
*/

#include <cstdint>
#include <memory>

#include <Engine/Graphics/UI/Font.hpp>
#include <Engine/Debug/Logger.hpp>

Font::Font() {}

Font::~Font()
{
    if (_loaded)
    {
        FT_Done_Face(_face);
    }
}

bool    Font::loadFromFile(const std::string &fileName)
{
    static std::unique_ptr<FT_Library> library = nullptr;
    int error;

    if (!library)
    {
        library = std::make_unique<FT_Library>();
        error = FT_Init_FreeType(library.get());
        CHECK_FT_ERROR(error, "Failed to init FreeType library");
    }

    error = FT_New_Face(*library.get(),
                        fileName.c_str(),
                        0,
                        &_face);

    if (error == FT_Err_Unknown_File_Format)
    {
        LOG_INFO("Unknow file format for %s", fileName.c_str());
    }
    CHECK_FT_ERROR(error, "Failed to init face for font %s", fileName.c_str());

    // The width will automaticaly be calculated based on the height
    error = FT_Set_Pixel_Sizes(_face, 0, 200);
    CHECK_FT_ERROR(error, "Failed to set pixel sizes for font %s", fileName.c_str());

    _chars.resize(255);
    for (uint32_t i = 0; i < 255; ++i)
    {
        error = FT_Load_Char(_face, i, FT_LOAD_RENDER);
        CHECK_FT_ERROR(error, "Failed to load char %c for font %s", i, fileName.c_str());

        if (!_face->glyph->bitmap.width || !_face->glyph->bitmap.rows)
        {
            LOG_WARN("Font::loadFromFile: Can't load char %d: texture width or height is 0", i);
            continue;
        }
        _chars[i].texture.load(_face->glyph->bitmap.width,
                            _face->glyph->bitmap.rows,
                            GL_RED,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            _face->glyph->bitmap.buffer);
        _chars[i].bearing = {_face->glyph->bitmap_left, _face->glyph->bitmap_top};
        _chars[i].advance = _face->glyph->advance.x;
    }

    CHECK_FT_ERROR(error, "Failed to set pixel sizes for font %s", fileName.c_str());


    _loaded = true;
    return (true);
}

const Font::sChar*   Font::getChar(char c) const
{
    if (c >= _chars.size())
        return (nullptr);
    return (&_chars[c]);
}
