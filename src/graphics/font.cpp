
#include <cstdio>
#include <cstring>

#include "stb/stb_truetype.h"

#include "game/render.h"
#include "graphics/font.h"

Font::Font(const char* path) :
        ttCharData(), quad(0.0f, 0.0f, 512.0f, 512.0f) {
    // Because these values are local, they are freed automatically when they fall out of scope
    unsigned char ttf_buffer[1 << 20];
    unsigned char pixels[512 * 512];

    // Use stb_truetype to read the font file and bake it to a texture
    std::fread(ttf_buffer, 1, 1 << 20, std::fopen(path, "rb"));
    stbtt_BakeFontBitmap(ttf_buffer, 0, 24.0, pixels, 512, 512, 32, 96, ttCharData);

    // Convert the bitmap to our internal texture format
    texture = new Texture({512, 512}, GL_RGBA8, GL_ALPHA, pixels, false, false, false);
}

Font::~Font() {
    delete texture;
}

vec2f Font::getSize(const char *str) const {
    // The size of 'output' is automatically shifted when calling 'stbtt_GetBakedQuad'
    vec2f output;
    stbtt_aligned_quad q;
    for (int i = 0; i < std::strlen(str); i++) {
        if (str[i] >= 32) {
            // Lookup the size and location of the current character
            float y;
            stbtt_GetBakedQuad(ttCharData, 512, 512, str[i] - 32, &output.x, &y, &q, 1);

            // We're only tracking the height of a single line, which is the maximum character height
            output.y = std::max(output.y, q.y1 - q.y0);
        }
    }
    output.x += q.x1 - q.x0; // Include the width of the last character
    return output;
}

void Font::draw(const Render& render, const char* str, vec2f location, Color color) {
    for (int i = 0; i < std::strlen(str); i++) {
        if (str[i] >= 32) {
            // Lookup the size and location of the current character
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(ttCharData, 512, 512, str[i] - 32, &location.x, &location.y, &q, 1);

            // Set the quad position and texture UVs based on current character
            quad.x = q.x0;
            quad.y = q.y0;
            quad.w = q.x1 - q.x0;
            quad.h = q.y1 - q.y0;
            quad.setUVs(q.s0, q.t0, q.s1, q.t1);

            // Draw to screen
            painter::draw(quad, *texture, render.shaderAlpha, color);
        }
    }
}
