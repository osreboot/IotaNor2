#pragma once

#include "stb/stb_truetype.h"

#include "graphics/painter.h"
#include "graphics/quad.h"
#include "types.h"

class Font {

private:
    Texture* texture;
    stbtt_bakedchar cdata[96];
    Quad quad;

public:

    Font();
    ~Font();

    Coordf getSize(const char* str) const;

    void render(const Render& render, const char* str, Coordf location, Color color);

};
