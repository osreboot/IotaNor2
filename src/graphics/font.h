#pragma once

#include "stb/stb_truetype.h"

#include "graphics/painter.h"
#include "graphics/quad.h"
#include "vec2f.h"

// Handles loading and rendering a single .ttf font
class Font {

private:
    Texture* texture; // The temporary texture created from baking the font vector graphic
    stbtt_bakedchar ttCharData[96]; // Character data used by stb_truetype
    Quad quad; // Quad representing a single character (reused many times each render call)

public:
    // Disable copying
    Font(Font const&) = delete;
    Font& operator=(Font const&) = delete;

    // Constructor requires the entire .ttf file path, including extension
    Font(const char* path);
    ~Font();

    // Returns the pixel size 'str' would be if drawn to the screen
    vec2f getSize(const char* str) const;

    // Renders 'str' to the screen
    void draw(const Render& render, const char* str, vec2f location, Color color);

};
