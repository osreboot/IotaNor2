
#include <cstdio>
#include <cstring>

#include "stb/stb_truetype.h"

#include "game/render.h"
#include "graphics/font.h"

Font::Font() : quad(0.0f, 0.0f, 512.0f, 512.0f){

    unsigned char ttf_buffer[1 << 20];
    unsigned char pixels[512 * 512];

    std::fread(ttf_buffer, 1, 1 << 20, std::fopen("../res/texture/ui/font/Rubik-Light.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer, 0, 24.0, pixels, 512, 512, 32, 96, cdata);

    texture = new Texture({512, 512}, GL_RGBA8, GL_ALPHA, pixels, false, false);
}

Font::~Font() {
    delete texture;
}

Coordf Font::getSize(const char *str) const {
    Coordf output = {0.0f, 0.0f};
    stbtt_aligned_quad q;
    for (int i = 0; i < std::strlen(str); i++) {
        if (str[i] >= 32) {
            float y;
            stbtt_GetBakedQuad(cdata, 512, 512, str[i] - 32, &output.first, &y, &q, 1);

            output.second = std::max(output.second, q.y1 - q.y0);
        }
    }
    output.first += q.x1 - q.x0;
    return output;
}

void Font::render(const Render& render, const char* str, Coordf location, Color color) {
    for (int i = 0; i < std::strlen(str); i++) {
        if (str[i] >= 32) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, str[i] - 32, &location.first, &location.second, &q, 1);

            quad.x = q.x0;
            quad.y = q.y0;
            quad.w = q.x1 - q.x0;
            quad.h = q.y1 - q.y0;
            quad.setUVs(q.s0, q.t0, q.s1, q.t1);

            painter::draw(quad, *texture, render.shaderAlpha, color);
        }
    }
}
