#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase; // Tile mask texture
uniform sampler2D textureBackground; // Background light texture
uniform sampler2D textureRefC, textureRefL, textureRefUL; // Refraction map textures

uniform vec4 textureColor; // Final colorization color

uniform vec2 windowSize, tileSize, tileLocation; // Coordinates describing the screen position of the tile

// This is the data necessary to translate between two vector coordinate spaces, where:
//   out = in * coef + bias
// Additionally, swap the input x and y values if 'swapped' is set.
struct Vec2Routing {
    vec2 coef;
    vec2 bias;
    bool swapped;
};

// Applies the routing equation to vector 'input'
vec2 route(vec2 input, Vec2Routing routing) {
    vec2 output = input * routing.coef + routing.bias;
    if(routing.swapped) output = vec2(output.y, output.x);
    return output;
}

// Re-maps the value 'x' from an input range to an output range (non-limiting)
vec2 map(vec2 x, vec2 inMin, vec2 inMax, vec2 outMin, vec2 outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Converts between screen coordinates and relative tile sampling coordinates
vec2 getBackgroundSampleLocation(vec2 tileSampleLocation) {
    return clamp(vec2(1.0), vec2(0.0), map(tileSampleLocation * tileSize + tileLocation, vec2(0.0), windowSize, vec2(0.0, 1.0), vec2(1.0, 0.0)));
}

// This function was initially far more complicated, however this was observed to produce the best visual result!
vec4 blend(vec4 color0, vec4 color1) {
    return vec4(color0.rgb + color1.rgb, 1.0);
}

// TLDR: Uses the refraction map 'sam' to calculate the color of light passed through the tile at the current fragment.
//
// More specifically, this function samples the refraction map texture at the current fragment UV transformed by the
// 'routingLookup' equation. This sampled color is then decoded into another UV value (this represents the position that
// the background light came from), which is then transformed by 'routingSample' into a final background sample
// location. The color sampled at this location is then multiplied by the blue channel of the initial refraction map
// sample, which represents the opacity of the glass at this location. This gives us the final refracted color for the
// fragment.
vec4 sampleRefractionMap(sampler2D sam, Vec2Routing routingLookup, Vec2Routing routingSample) {
    vec4 sampleLookup = texture(sam, route(uv, routingLookup));
    vec4 colorRefracted = texture(textureBackground, getBackgroundSampleLocation(route(vec2(sampleLookup.g / sampleLookup.b, sampleLookup.r / sampleLookup.b), routingSample)));
    colorRefracted *= sampleLookup.b;
    return colorRefracted;
}

void main() {
    // Calculate light refracted from directly behind the tile
    vec4 colorRefC = sampleRefractionMap(textureRefC, Vec2Routing(vec2(1.0), vec2(0.0), false), Vec2Routing(vec2(1.0), vec2(0.0), false));

    // Calculate light refracted from behind adjacent tiles
    vec4 colorRefL = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), false), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, 0.0), false));
    vec4 colorRefR = sampleRefractionMap(textureRefL, Vec2Routing(vec2(-1.0, 1.0), vec2(1.0, 0.0), false), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, 0.0), false));
    vec4 colorRefU = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), true), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, 0.0), true));
    vec4 colorRefD = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, -1.0), vec2(0.0, 1.0), true), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, 0.0), true));

    vec4 colorRefAdj = blend(blend(colorRefL, colorRefR), blend(colorRefU, colorRefD));

    // Calculate light refracted from behind diagonally adjacent tiles
    vec4 colorRefUL = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), false), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, -1.0), false));
    vec4 colorRefUR = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(-1.0, 1.0), vec2(1.0, 0.0), false), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, -1.0), false));
    vec4 colorRefDL = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(1.0, -1.0), vec2(0.0, 1.0), false), Vec2Routing(vec2(1.0, -1.0), vec2(-1.0, 2.0), false));
    vec4 colorRefDR = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(-1.0, -1.0), vec2(1.0, 1.0), false), Vec2Routing(vec2(-1.0, -1.0), vec2(2.0, 2.0), false));

    vec4 colorRefDiag = blend(blend(colorRefUL, colorRefUR), blend(colorRefDL, colorRefDR));

    // Blend all refracted light
    color = blend(colorRefC, blend(colorRefAdj, colorRefDiag));

    // Apply the tile outline mask and texture colorization
    color *= vec4(1.0, 1.0, 1.0, texture(textureBase, uv).r) * textureColor;
}
