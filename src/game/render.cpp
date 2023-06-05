
#include <string>
#include "game/game.h"
#include "game/render.h"
#include "graphics/display.h"
#include "util.h"

const Color Render::WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
const Color Render::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

Render::Render(Game& game) :
        shaderDefault("res/shader/default.vert", "res/shader/default.frag", [](const GLuint&){}),
        shaderAlpha("res/shader/default.vert", "res/shader/alpha_colorize.frag", [](const GLuint&){}),
        shaderRefract("res/shader/default.vert", "res/shader/tile_refract.frag", [&](const GLuint& idProgram){
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
        shaderFire("res/shader/default.vert", "res/shader/fire.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboFire0.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire0"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, fboFire1.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureFire1"), 2);

            glUniform1f(glGetUniformLocation(idProgram, "intensity"),visFireIntensity);
        }),
        shaderMask("res/shader/default.vert", "res/shader/mask.frag", [&](const GLuint& idProgram){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fboMaskChannel0.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureChannel0"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, fboMaskChannel1.getId());
            glUniform1i(glGetUniformLocation(idProgram, "textureChannel1"), 2);
        }),
        fboFire0(TextureFBO::build(display::getSize())),
        fboFire1(TextureFBO::build(display::getSize())),
        fboGameContent(TextureFBO::build(display::getSize())),
        fboRefractedContent(TextureFBO::build(display::getSize())),
        fboMask(TextureFBO::build(display::getSize())),
        fboMaskChannel0(TextureFBO::build(display::getSize())),
        fboMaskChannel1(TextureFBO::build(display::getSize())),
        textureTest(Texture::load("res/texture/ui/test.png")),
        textureSolid(Texture::load("res/texture/ui/solid.png")),
        textureNoise(Texture::load("res/texture/ui/noise.png")),
        textureTileMask(Texture::load("res/texture/material/tile_mask.png")),
        textureTileRefC(Texture::load("res/texture/material/tile_ref_4c.png")),
        textureTileRefL(Texture::load("res/texture/material/tile_ref_4l.png")),
        textureTileRefUL(Texture::load("res/texture/material/tile_ref_4ul.png")),
        textureUiTileLit(Texture::load("res/texture/ui/tile_highlighted.png")),
        textureUiTileSwap(Texture::load("res/texture/ui/tile_swap.png")),
        textureUiTileSwapSmall(Texture::load("res/texture/ui/tile_swap_small.png")),
        textureUiCircle(Texture::load("res/texture/ui/circle.png")),
        textureUiQueueBox(Texture::load("res/texture/ui/queue_outline.png")),
        textureUiAudioMuted(Texture::load("res/texture/ui/mute.png")),
        textureBoardMask(Texture::load("res/texture/ui/board_mask.png")),
        quadScreen(0.0f, 0.0f, static_cast<float>(display::getSize().first), static_cast<float>(display::getSize().second)),
        quadUiStageS(0.0f, 0.0f, 6.0f, 6.0f),
        quadUiStageL(0.0f, 0.0f, 8.0f, 8.0f),
        quadUiQueueBox(0.0f, 0.0f, Tile::SIZE * 4.0f, Tile::SIZE * 4.0f),
        quadUiAudioMuted(0.0f, 0.0f, 64.0f, 64.0f),
        quadTileInfection(0.0f, 0.0f, 0.0f, 0.0f),
        quadBoardMask(0.0f, 0.0f, 0.0f, 0.0f),
        font(),
        progressBar(),
        visFireIntensity(0.0f) {}

void Render::render(float delta, Game& game) {
    timer += delta;

    // Update tile visuals
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            Tile& tile = game.tiles[x][y];
            tile.visTimerLastUpdate += delta;
            stepTowards(tile.visTimerShock, delta * 4.0f, 0.0f);

            const float sizeEasing = 1.0f - 4.0f * powf(std::clamp(tile.visTimerShock, 0.0f, 1.0f) - 0.5f, 2.0f);
            const float size = Tile::SIZE - sizeEasing * 8.0f;
            const Coordf world = Game::getWorld({x, y});
            tile.quad.x = world.first + (Tile::SIZE / 2.0f) - (size / 2.0f);
            tile.quad.y = world.second + (Tile::SIZE / 2.0f) - (size / 2.0f);
            tile.quad.w = size;
            tile.quad.h = size;
        }
    }

    // Capture fire elements
    stepTowards(visFireIntensity, delta / 3.0f, static_cast<float>(game.stage) / static_cast<float>(Game::STAGES - 1));
    quadScreen.setUVs((timer / 32.0f), (timer / 16.0f), (timer / 32.0f) + 1.0f, (timer / 16.0f) + 0.4f);
    fboFire0.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, WHITE);
    });
    quadScreen.setUVs((timer / 24.0f) + 1.2f, 0.0f, (timer / 24.0f), 0.4f);
    fboFire1.capture(true, [&](){
        painter::draw(quadScreen, textureNoise, shaderDefault, WHITE);
    });
    quadScreen.setUVs(0.0f, 0.0f, 1.0f, 1.0f);

    // Capture game elements
    fboGameContent.capture(true, [&](){
        // Tiles
        for (int x = 0; x < Game::BOARD_DIM; x++) {
            for (int y = 0; y < Game::BOARD_DIM; y++) {
                Tile& tile = game.tiles[x][y];
                if (tile.isIlluminated()) painter::draw(tile.quad, textureTileMask, shaderDefault, WHITE);

                if (tile.timerInfect > 0.0f) {
                    const float intensityInfection = 1.0f - powf(1.0f - (tile.timerInfect / game.getStageInfectTime()), 4.0f);
                    const float sizeInfection = map(intensityInfection, 1.0f, 0.0f, 8.0f, tile.quad.w);
                    Coordi tileWorld = Game::getWorld({x, y});
                    quadTileInfection.x = static_cast<float>(tileWorld.first) + (Tile::SIZE / 2.0f) - (sizeInfection / 2.0f);
                    quadTileInfection.y = static_cast<float>(tileWorld.second) + (Tile::SIZE / 2.0f) - (sizeInfection / 2.0f);
                    quadTileInfection.w = sizeInfection;
                    quadTileInfection.h = sizeInfection;
                    painter::draw(quadTileInfection, textureTileMask, shaderDefault, tile.isIlluminated() ? BLACK : WHITE);
                }
            }
        }

        // Queue and hold box outlines
        quadUiQueueBox.x = Game::getOriginQueue(0).first - (quadUiQueueBox.w / 2.0f);
        quadUiQueueBox.y = Game::getOriginQueue(0).second + (Tile::SIZE / 2.0f) - (quadUiQueueBox.h / 2.0f);
        painter::draw(quadUiQueueBox, textureUiQueueBox, shaderDefault, WHITE);
        quadUiQueueBox.x = Game::getOriginHold().first - (quadUiQueueBox.w / 2.0f);
        quadUiQueueBox.y = Game::getOriginHold().second + (Tile::SIZE / 2.0f) - (quadUiQueueBox.h / 2.0f);
        painter::draw(quadUiQueueBox, textureUiQueueBox, shaderDefault, WHITE);

        if (!game.frozen) {
            // Progress bar
            progressBar.render(delta, *this, game);

            // Stage pips
            for (int i = 0; i < Game::STAGES - 1; i++) {
                quadUiStageL.x = (static_cast<float>(display::getSize().first) / 2.0f) - (quadUiStageL.w / 2.0f) -
                                 (static_cast<float>(Game::STAGES) / 2.0f) * 64.0f + (static_cast<float>(i + 1) * 64.0f);
                quadUiStageL.y = static_cast<float>(display::getSize().second) - 48.0f - (quadUiStageL.h / 2.0f);
                painter::draw(quadUiStageL, textureUiCircle, shaderDefault, WHITE);
                quadUiStageS.x = (static_cast<float>(display::getSize().first) / 2.0f) - (quadUiStageS.w / 2.0f) -
                                 (static_cast<float>(Game::STAGES) / 2.0f) * 64.0f + (static_cast<float>(i + 1) * 64.0f);
                quadUiStageS.y = static_cast<float>(display::getSize().second) - 48.0f - (quadUiStageS.h / 2.0f);
                if(game.stage <= i) painter::draw(quadUiStageS, textureUiCircle, shaderDefault, BLACK);
            }
        }

        // Text
        game.stats.render(*this, game, font);

        // Audio muted icon
        if (game.audio.muted) {
            quadUiAudioMuted.x = 16.0f;
            quadUiAudioMuted.y = static_cast<float>(display::getSize().second) - quadUiAudioMuted.h - 16.0f;
            painter::draw(quadUiAudioMuted, textureUiAudioMuted, shaderDefault, {1.0f, 1.0f, 1.0f, 0.6f});
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
            Tile& tile = game.tiles[x][y];
            currentTileQuad = &tile.quad;
            painter::draw(tile.quad, textureTileMask, shaderRefract, WHITE);

            const float glintIntensity = 1.0f - 4.0f * powf(std::clamp(tile.visTimerShock, 0.0f, 1.0f) - 0.5f, 2.0f);
            if (glintIntensity > 0.0f)
                painter::draw(tile.quad, textureUiTileLit, shaderDefault, {1.0f, 1.0f, 1.0f, glintIntensity});
        }
    }

    // Capture floating piece groups (glass tiles and swap icons)
    quadBoardMask.w = Game::BOARD_DIM * Tile::SIZE * (2048.0f / (2048.0f - 128.0f));
    quadBoardMask.h = Game::BOARD_DIM * Tile::SIZE * (2048.0f / (2048.0f - 128.0f));
    quadBoardMask.x = (static_cast<float>(display::getSize().first) / 2.0f) - (quadBoardMask.w / 2.0f);
    quadBoardMask.y = (static_cast<float>(display::getSize().second) / 2.0f) - (quadBoardMask.h / 2.0f);
    fboMask.capture(true, [&](){
        painter::draw(quadBoardMask, textureBoardMask, shaderDefault, WHITE);
    });

    fboMaskChannel0.capture(true, [&](){
        for (Group* group : game.groups) render(delta, *group, false);
        if (game.groupHold) render(delta, *game.groupHold, false);
    });
    fboMaskChannel1.capture(true, [&](){});
    painter::draw(quadScreen, fboMask, shaderMask, WHITE);

    fboMaskChannel0.capture(true, [&](){});
    fboMaskChannel1.capture(true, [&](){
        for (Group* group : game.groups) render(delta, *group, true);
        if (game.groupHold) render(delta, *game.groupHold, true);
    });
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    painter::draw(quadScreen, fboMask, shaderMask, WHITE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::render(float delta, Group& group, bool channelBoard) {
    for (int x = 0; x < Group::DIM; x++) {
        for (int y = 0; y < Group::DIM; y++) {
            TileFloating* const tile = group.tiles[y * Group::DIM + x];

            if (tile) {
                tile->update(delta, group.location, {x, y});

                Coordf tileVisLocation = tile->getVisLocation(group.location, {x, y});
                tile->quad.x = tileVisLocation.first;
                tile->quad.y = tileVisLocation.second;

                if (channelBoard) {
                    const bool centerTile = x == Group::DIM / 2 && y == Group::DIM / 2;
                    painter::draw(tile->quad, centerTile ? textureUiTileSwap : textureUiTileSwapSmall, shaderDefault, WHITE);
                } else {
                    currentTileQuad = &tile->quad;
                    painter::draw(tile->quad, textureTileMask, shaderRefract, WHITE);
                    //painter::draw(tile->quad, textureUiTileLit, shaderDefault, WHITE);
                }
            }
        }
    }
}
