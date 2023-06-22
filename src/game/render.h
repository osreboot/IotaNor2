#pragma once

#include "game/ui/progress_bar.h"
#include "graphics/font.h"
#include "graphics/painter.h"
#include "graphics/texture_frame_buffer.h"
#include "game/group.h"

// Class that manages the rendering cycle for all game elements
class Render {

private:
    Shader shaderFire; // Combines fire noise textures and game elements to produce fire-y game elements
    Shader shaderRefract; // Creates those nice tile refractions, given the appropriate maps and background content
    Shader shaderMask; // A simple shader that interpolates between two textures, based on a mask texture input

    TextureFBO fboFire0, fboFire1; // FBOs for the fire noise textures
    TextureFBO fboGameContent, fboRefractedContent; // FBOs to capture elements that are refracted by the tile shader
    TextureFBO fboMask, fboMaskChannel0, fboMaskChannel1; // FBOs for the masking shader

    // Debug and generic textures
    Texture textureTest, textureSolid, textureNoise;

    // Various UI textures
    Texture textureUiTileSwap, textureUiTileSwapSmall, textureUiCircle, textureUiQueueBox, textureUiAudioMuted;

    // Refraction map and tile-specific textures
    Texture textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL, textureTileLit;

    // Texture for fading between the refraction shader and piece placement UI overlay
    Texture textureBoardMask;

    Quad quadDebugCursor; // Quad drawn around the cursor, only for debug purposes
    Quad quadScreen; // Quad the size of the screen
    Quad quadUiStageS, quadUiStageL, quadUiQueueBox, quadUiAudioMuted; // Quads for verious UI elements
    Quad quadTileInfection; // Quad for tile infection textures
    Quad quadBoardMask; // Quad for fading between the refraction shader and piece placement UI overlay

    ProgressBar progressBar;
    Font font;

    float visFireIntensity; // Visual intensity of the fire effect, scales dynamically with stage number

    // Data consumed by the tile refraction shader
    bool refractSendTextures = false;
    const Quad* refractQuad = nullptr;

    // Render a single group of tiles, 'channelBoard' is set if we are drawing glassy tiles, otherwise draw swap icons
    void render(float delta, Group& group, bool channelBoard);

public:
    static const Color WHITE;
    static const Color BLACK;

    float timer = 0.0f; // Simple timer that always counts up, used for advancing some animations

    Shader shaderDefault; // Shader that draws a texture without any effects
    Shader shaderAlpha; // Shader that copies the alpha channel from another input texture

    // Disable copying, because we have a lot of members that can't be copied
    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    Render();

    // Advances animations by 'delta' seconds, and renders all game elements
    void render(float delta, Game& game);

};
