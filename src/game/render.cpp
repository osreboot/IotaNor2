
#include "render.h"
#include "../display.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
const Color Render::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

Render::Render() :
        shaderDefault("../res/default.vert", "../res/default.frag", [](const GLuint&){}),
        shaderRefract("../res/default.vert", "../res/tile_refract.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboRefractedContent.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureBackground"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureTileRefC.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureRefC"), 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, textureTileRefL.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureRefL"), 3);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, textureTileRefUL.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureRefUL"), 4);

            glUniform2f(glGetUniformLocation(idProgram, "windowSize"),
                        static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second));
            glUniform2f(glGetUniformLocation(idProgram, "tileSize"), currentTileQuad->w, currentTileQuad->h);
            glUniform2f(glGetUniformLocation(idProgram, "tileLocation"), currentTileQuad->x, currentTileQuad->y);
        }),
        shaderFire("../res/default.vert", "../res/fire.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboFire0.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire0"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, fboFire1.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire1"), 2);
        }),
        fboFire0(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        fboFire1(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        fboGameContent(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        fboRefractedContent(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        textureTest(Texture::load("../res/test.png")),
        textureNoise(Texture::load("../res/noise.png")),
        textureTileMask(Texture::load("../res/tile_mask.png")),
        textureTileRefC(Texture::load("../res/tile_ref_7c.png")),
        textureTileRefL(Texture::load("../res/tile_ref_7l.png")),
        textureTileRefUL(Texture::load("../res/tile_ref_7ul.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)) {

}

void Render::render(float delta, const Game& game) {
    timer += delta;

    // Capture fire elements
    quadScreen.setUVs((timer / 40.0f), (timer / 20.0f), (timer / 40.0f) + 1.0f, (timer / 20.0f) + 0.4f);
    fboFire0.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, WHITE);
    });
    quadScreen.setUVs((timer / 32.0f) + 1.2f, 0.0f, (timer / 32.0f), 0.4f);
    fboFire1.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, WHITE);
    });
    quadScreen.setUVs(0.0f, 0.0f, 1.0f, 1.0f);

    // Capture game elements
    fboGameContent.capture(true, [&](){
        for(const Tile& tile : game.tiles) {
            if(tile.illum) painter::draw(tile.quad, textureTileMask, shaderDefault, WHITE);
        }
    });

    // Capture all background elements (so they can be processed by the tile refraction shader)
    fboRefractedContent.capture(true, [&](){
        painter::draw(quadScreen, fboGameContent, shaderFire, WHITE);
        painter::draw(game.quadCursor, textureTest, shaderDefault, WHITE);
    });

    // Display all background elements
    painter::draw(quadScreen, fboRefractedContent, shaderDefault, WHITE);

    // Display tiles
    for(const Tile& tile : game.tiles) {
        currentTileQuad = &tile.quad;
        painter::draw(tile.quad, textureTileMask, shaderRefract, WHITE);
    }

}

Render::~Render() {}
