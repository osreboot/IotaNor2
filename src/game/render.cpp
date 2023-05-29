
#include "render.h"
#include "../display.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

Render::Render() :
        shaderDefault("../res/default_vert.vert", "../res/default_frag.frag", [](const GLuint&){}),
        shaderRefract("../res/default_vert.vert", "../res/tile_refract.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, frameBufferBackground.getId());
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
            glUniform2f(glGetUniformLocation(idProgram, "tileSize"), Tile::SIZE, Tile::SIZE);
            glUniform2f(glGetUniformLocation(idProgram, "tileLocation"), currentTileX, currentTileY);
        }),
        frameBufferBackground(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        textureTest(Texture::load("../res/test.png")),
        textureTileMask(Texture::load("../res/tile_mask.png")),
        textureTileRefC(Texture::load("../res/tile_ref_7c.png")),
        textureTileRefL(Texture::load("../res/tile_ref_7l.png")),
        textureTileRefUL(Texture::load("../res/tile_ref_7ul.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)) {

}

void Render::render(const Game& game) {

    frameBufferBackground.capture(true, [&](){
        painter::draw(game.quadCursor, textureTest, shaderDefault, WHITE);
    });

    painter::draw(quadScreen, frameBufferBackground, shaderDefault, {1.0f, 1.0f, 1.0f, 0.5f});

    for(const Tile& tile : game.tiles) {
        currentTileX = static_cast<float>(tile.quad.x);
        currentTileY = static_cast<float>(tile.quad.y);
        painter::draw(tile.quad, textureTileMask, shaderRefract, WHITE);
    }

}

Render::~Render() {}
