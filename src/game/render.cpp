
#include "render.h"
#include "../texture_frame_buffer.h"
#include "../display.h"

namespace render {

    void initialize() {
        shaderDefault = new Shader("../res/default_vert.vert", "../res/default_frag.frag", [](const GLuint&){});
        fbo0 = TextureFBO::build(static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second));
    }

    void render(Game& game) {

        fbo0->capture(true, [&](){
            painter::draw(game.quadCursor, game.textureTest, *shaderDefault, WHITE);
        });

        for(const Tile& tile : game.tiles) {
            painter::draw(tile.quad, game.textureTile, *shaderDefault, WHITE);
        }

        painter::draw(game.quadCursor, *fbo0, *shaderDefault, WHITE);

    }

    void cleanup() {
        delete shaderDefault;
        delete fbo0;
    }

}
