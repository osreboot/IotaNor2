
#include "game/render.h"
#include "graphics/display.h"
#include "util.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
const Color Render::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

Render::Render(Game& game) :
        shaderDefault("../res/shader/default.vert", "../res/shader/default.frag", [](const GLuint&){}),
        shaderRefract("../res/shader/default.vert", "../res/shader/tile_refract.frag", [&](const GLuint& idProgram){
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
        shaderFire("../res/shader/default.vert", "../res/shader/fire.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboFire0.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire0"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, fboFire1.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire1"), 2);
        }),
        fboFire0(TextureFBO::build(display::getSize())),
        fboFire1(TextureFBO::build(display::getSize())),
        fboGameContent(TextureFBO::build(display::getSize())),
        fboRefractedContent(TextureFBO::build(display::getSize())),
        textureTest(Texture::load("../res/texture/ui/test.png")),
        textureNoise(Texture::load("../res/texture/ui/noise.png")),
        textureTileMask(Texture::load("../res/texture/material/tile_mask.png")),
        textureTileRefC(Texture::load("../res/texture/material/tile_ref_4c.png")),
        textureTileRefL(Texture::load("../res/texture/material/tile_ref_4l.png")),
        textureTileRefUL(Texture::load("../res/texture/material/tile_ref_4ul.png")),
        textureUiTileLit(Texture::load("../res/texture/ui/tile_highlighted.png")),
        textureUiTileSwap(Texture::load("../res/texture/ui/tile_swap.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)) {

}

void Render::render(float delta, Game& game) {
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
        for (int x = 0; x < Game::BOARD_DIM; x++) {
            for (int y = 0; y < Game::BOARD_DIM; y++) {
                if (game.tiles[x][y].illuminated) painter::draw(game.tiles[x][y].quad, textureTileMask, shaderDefault, WHITE);
            }
        }
    });

    // Capture all background elements (so they can be processed by the tile refraction shader)
    fboRefractedContent.capture(true, [&](){
        painter::draw(quadScreen, fboGameContent, shaderFire, WHITE);
        //painter::draw(game.quadDebugCursor, textureTest, shaderDefault, WHITE);
    });

    // Display all background elements
    painter::draw(quadScreen, fboRefractedContent, shaderDefault, WHITE);

    // Display tiles
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            currentTileQuad = &game.tiles[x][y].quad;
            painter::draw(game.tiles[x][y].quad, textureTileMask, shaderRefract, WHITE);
        }
    }

    for (Group& group : game.groups) render(group);
}

void Render::render(Group& group) {
    for (int x = 0; x < Group::DIM; x++) {
        for (int y = 0; y < Group::DIM; y++) {
            Tile* const tile = group.tiles[y * Group::DIM + x];

            if (tile) {
                tile->quad.x = group.location.first - (static_cast<float>(Group::DIM) * Tile::SIZE / 2.0f) +
                        static_cast<float>(x) * Tile::SIZE;
                tile->quad.y = group.location.second - (static_cast<float>(Group::DIM) * Tile::SIZE / 2.0f) +
                               static_cast<float>(y) * Tile::SIZE;

                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                painter::draw(tile->quad, textureUiTileSwap, shaderDefault, WHITE);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                painter::draw(tile->quad, textureUiTileLit, shaderDefault, WHITE);

                //currentTileQuad = &tile->quad;
                //painter::draw(tile->quad, textureTileMask, shaderRefract, WHITE);
            }
        }
    }
}
