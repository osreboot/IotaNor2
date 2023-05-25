
#include <iostream>
#include "render.h"

using namespace std;

void render::render(Game& game) {

    painter::draw(game.quadCursor, game.textureTest, WHITE);

    for(const Tile& tile : game.tiles) {
        painter::draw(tile.quad, game.textureTile, WHITE);
    }

}
