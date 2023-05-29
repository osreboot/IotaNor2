
#include "render.h"
#include "../display.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

Render::Render() :
        shaderDefault("../res/default_vert.vert", "../res/default_frag.frag", [](const GLuint&){}),
        shaderRefract("../res/default_vert.vert", "../res/tile_refract.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboBackground.getId());
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
        shaderBackgroundFire("../res/default_vert.vert", "../res/background_fire.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboBackgroundFire1.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureBase1"), 1);
        }),
        fboBackgroundFire0(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        fboBackgroundFire1(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        fboBackground(TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second))),
        textureTest(Texture::load("../res/test.png")),
        textureNoise(Texture::load("../res/noise.png")),
        textureTileMask(Texture::load("../res/tile_mask.png")),
        textureTileRefC(Texture::load("../res/tile_ref_2c.png")),
        textureTileRefL(Texture::load("../res/tile_ref_2l.png")),
        textureTileRefUL(Texture::load("../res/tile_ref_2ul.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)) {

}

void Render::render(float delta, const Game& game) {
    timer += delta;

    quadScreen.setUVs((timer / 40.0f), (timer / 20.0f), (timer / 40.0f) + 1.0f, (timer / 20.0f) + 0.4f);
    fboBackgroundFire0.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, {0.0f, 0.0f, 1.0f, 1.0f});
    });
    quadScreen.setUVs((timer / 32.0f) + 1.2f, 0.0f, (timer / 32.0f), 0.4f);
    fboBackgroundFire1.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, {0.0f, 0.0f, 1.0f, 1.0f});
    });
    quadScreen.setUVs(0.0f, 0.0f, 1.0f, 1.0f);

    fboBackground.capture(true, [&](){
        painter::draw(quadScreen, fboBackgroundFire0, shaderBackgroundFire, WHITE);
        painter::draw(game.quadCursor, textureTest, shaderDefault, WHITE);
    });

    painter::draw(quadScreen, fboBackground, shaderDefault, {1.0f, 1.0f, 1.0f, 1.0f});

    for(const Tile& tile : game.tiles) {
        currentTileX = static_cast<float>(tile.quad.x);
        currentTileY = static_cast<float>(tile.quad.y);
        painter::draw(tile.quad, textureTileMask, shaderRefract, WHITE);
    }

}

Render::~Render() {}
