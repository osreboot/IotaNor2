#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase, textureBackground, textureRefC, textureRefL, textureRefUL;
uniform vec4 textureColor;
uniform vec2 windowSize, tileSize, tileLocation;

struct Vec2Routing {
    vec2 coef;
    vec2 bias;
    bool swapped;
};

vec2 route(vec2 input, Vec2Routing routing) {
    vec2 output = input * routing.coef + routing.bias;
    if(routing.swapped) output = vec2(output.y, output.x);
    return output;
}

vec2 map(vec2 x, vec2 inMin, vec2 inMax, vec2 outMin, vec2 outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

vec2 getBackgroundSampleLocation(vec2 tileSampleLocation) {
    return clamp(vec2(1.0), vec2(0.0), map(tileSampleLocation * tileSize + tileLocation, vec2(0.0), windowSize, vec2(0.0, 1.0), vec2(1.0, 0.0)));
}

vec4 blend(vec4 color1, vec4 color0) {
    return vec4(color1.rgb + color0.rgb, 1.0);
    //return vec4(max(color1.r, color0.r), max(color1.g, color0.g), max(color1.b, color0.b), 1.0);

    // Source: https://stackoverflow.com/questions/28900598/how-to-combine-two-colors-with-varying-alpha-values
/*float a01 = (1.0 - color0.a) * color1.a + color0.a;
    float r01 = (((1.0 - color0.a) * color1.a * color1.r) + (color0.a * color0.r)) / a01;
    float g01 = (((1.0 - color0.a) * color1.a * color1.g) + (color0.a * color0.g)) / a01;
    float b01 = (((1.0 - color0.a) * color1.a * color1.b) + (color0.a * color0.b)) / a01;
    return vec4(r01, g01, b01, a01);*/
    //return colorBottom * (1.0 - colorTop.a) + vec4(colorTop.rgb * colorTop.a, colorTop.a);
}

vec4 sampleRefractionMap(sampler2D sam, Vec2Routing routingLookup, Vec2Routing routingSample) {
    vec4 sampleLookup = texture(sam, route(uv, routingLookup));
    vec4 colorRefracted = texture(textureBackground, getBackgroundSampleLocation(route(vec2(sampleLookup.g / sampleLookup.b, sampleLookup.r / sampleLookup.b), routingSample)));
    colorRefracted *= sampleLookup.b;
    return colorRefracted;
}

void main() {
    vec4 colorRefC = sampleRefractionMap(textureRefC, Vec2Routing(vec2(1.0), vec2(0.0), false), Vec2Routing(vec2(1.0), vec2(0.0), false));

    vec4 colorRefL = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), false), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, 0.0), false));
    vec4 colorRefR = sampleRefractionMap(textureRefL, Vec2Routing(vec2(-1.0, 1.0), vec2(1.0, 0.0), false), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, 0.0), false));
    vec4 colorRefU = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), true), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, 0.0), true));
    vec4 colorRefD = sampleRefractionMap(textureRefL, Vec2Routing(vec2(1.0, -1.0), vec2(0.0, 1.0), true), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, 0.0), true));

    vec4 colorRefAdj = blend(blend(colorRefL, colorRefR), blend(colorRefU, colorRefD));

    vec4 colorRefUL = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(1.0, 1.0), vec2(0.0, 0.0), false), Vec2Routing(vec2(1.0, 1.0), vec2(-1.0, -1.0), false));
    vec4 colorRefUR = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(-1.0, 1.0), vec2(1.0, 0.0), false), Vec2Routing(vec2(-1.0, 1.0), vec2(2.0, -1.0), false));
    vec4 colorRefDL = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(1.0, -1.0), vec2(0.0, 1.0), false), Vec2Routing(vec2(1.0, -1.0), vec2(-1.0, 2.0), false));
    vec4 colorRefDR = sampleRefractionMap(textureRefUL, Vec2Routing(vec2(-1.0, -1.0), vec2(1.0, 1.0), false), Vec2Routing(vec2(-1.0, -1.0), vec2(2.0, 2.0), false));

    vec4 colorRefDiag = blend(blend(colorRefUL, colorRefUR), blend(colorRefDL, colorRefDR));

    color = blend(colorRefC, blend(colorRefAdj, colorRefDiag));
    //color = blend(color, vec4(0.02, 0.02, 0.02, 1.0));
    color *= vec4(1.0, 1.0, 1.0, texture(textureBase, uv).r) * textureColor;
}
