
#include "game/game.h"
#include "game/ui/progress_bar.h"
#include "game/render.h"
#include "graphics/display.h"
#include "graphics/painter.h"
#include "util.h"

const float ProgressBar::W = 256.0f;

ProgressBar::ProgressBar() :
        textureEnds(Texture::load("../res/texture/ui/progress/progress_ends_c.png")),
        textureEndsBack(Texture::load("../res/texture/ui/progress/progress_ends_cb.png")),
        textureCutout(Texture::load("../res/texture/ui/progress/progress_cutout.png")),
        textureBlackGrow(Texture::load("../res/texture/ui/progress/progress_black_grow.png")),
        textureWhiteGrow(Texture::load("../res/texture/ui/progress/progress_white_grow.png")),
        textureBlackShrink(Texture::load("../res/texture/ui/progress/progress_black_shrink.png")),
        textureWhiteShrink(Texture::load("../res/texture/ui/progress/progress_white_shrink.png")),
        quad(0.0f, 0.0f, W, W / 16.0f),
        quadBlack(0.0f, 0.0f, W, W / 16.0f),
        quadWhite(0.0f, 0.0f, W, W / 16.0f),
        visProgress(0.0f) {}

void ProgressBar::render(float delta, const Render& render, const Game &game) {
    int numIlluminated = 0;
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            if (game.tiles[x][y].isIlluminated()) numIlluminated++;
        }
    }
    float progress = static_cast<float>(numIlluminated) / static_cast<float>(Game::BOARD_DIM * Game::BOARD_DIM);

    stepTowards(visProgress, delta / 2.0f, progress);

    quad.x = (static_cast<float>(display::getSize().first) / 2.0f) - (W / 2.0f);
    quad.y = 48.0f - (W / 32.0f) * 0.75f;
    painter::draw(quad, textureEndsBack, render.shaderDefault, Render::WHITE);

    quadWhite.x = quad.x + (W * (1.0f - visProgress));
    quadWhite.y = quad.y;
    quadWhite.w = W * visProgress;
    quadWhite.u1 = quadWhite.w / W;
    painter::draw(quadWhite, game.getStageGoal() ? textureWhiteGrow : textureWhiteShrink, render.shaderDefault, Render::WHITE);

    quadBlack.x = quad.x;
    quadBlack.y = quad.y;
    quadBlack.w = W * (1.0f - visProgress);
    quadBlack.u0 = 1.0f - (quadBlack.w / W);
    painter::draw(quadBlack, game.getStageGoal() ? textureBlackShrink : textureBlackGrow, render.shaderDefault, Render::WHITE);

    painter::draw(quad, textureCutout, render.shaderDefault, Render::WHITE);
    painter::draw(quad, textureEnds, render.shaderDefault, Render::WHITE);
}
