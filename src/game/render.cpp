
#include "render.h"
#include "../display.h"
#include "../util.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
const Color Render::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

Render::Render(Game& game) :
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
        textureTileRefC(Texture::load("../res/tile_ref_4c.png")),
        textureTileRefL(Texture::load("../res/tile_ref_4l.png")),
        textureTileRefUL(Texture::load("../res/tile_ref_4ul.png")),
        textureTileHighlighted(Texture::load("../res/tile_highlighted.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)) {

    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            Tile& tile = game.tiles[x][y];

            std::pair<float, float> world = Game::getWorld(x, y);
            tile.quadLocked.w = tile.visualSize;
            tile.quadLocked.h = tile.visualSize;
            tile.quadLocked.x = world.first;
            tile.quadLocked.y = world.second;
        }
    }

}

void Render::render(float delta, Game& game) {
    timer += delta;

    // Update tile visuals
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            Tile& tile = game.tiles[x][y];
            stepTowards(tile.visualSize, delta * 200.0f, Tile::SIZE);

            std::pair<float, float> world = Game::getWorld(x, y);
            tile.quad.w = tile.visualSize;
            tile.quad.h = tile.visualSize;
            tile.quad.x = world.first + (Tile::SIZE / 2.0f) - (tile.visualSize / 2.0f);
            tile.quad.y = world.second + (Tile::SIZE / 2.0f) - (tile.visualSize / 2.0f);
        }
    }

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
        painter::draw(game.quadDebugCursor, textureTest, shaderDefault, WHITE);
    });

    // Display all background elements
    painter::draw(quadScreen, fboRefractedContent, shaderDefault, WHITE);

    // Display tiles (separated to draw larger tiles on top)
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            if (game.tiles[x][y].visualSize <= Tile::SIZE) {
                currentTileQuad = &game.tiles[x][y].quad;
                painter::draw(game.tiles[x][y].quad, textureTileMask, shaderRefract, WHITE);
            }
        }
    }
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            if (game.tiles[x][y].visualSize > Tile::SIZE) {
                currentTileQuad = &game.tiles[x][y].quad;
                painter::draw(game.tiles[x][y].quad, textureTileMask, shaderRefract, WHITE);
            }
        }
    }

    painter::draw(game.quadCursor, textureTileHighlighted, shaderDefault, WHITE);

}

Render::~Render() {}
