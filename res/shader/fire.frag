#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase, textureFire0, textureFire1;
uniform vec4 textureColor;

void main() {
    vec4 colorBase = texture(textureBase, uv);
    if (colorBase.r < 0.5) {
        // Red (on black) fire rising from floor
        float valueFire = texture(textureFire0, uv).r * texture(textureFire1, uv).r;
        valueFire = 1.0 - pow(1.0 - valueFire, 4.0); // Quadratic easing function for more intense color
        color = (1.0 - texture(textureBase, uv)) * vec4(valueFire * -uv.y, 0.0, 0.0, 1.0);
    } else {
        // Blue (on white) fire falling from ceiling
        float valueFire = texture(textureFire0, 1.0 - uv).r * texture(textureFire1, 1.0 - uv).r;
        valueFire = 1.0 - pow(1.0 - valueFire, 4.0); // Quadratic easing function for more intense color
        color = texture(textureBase, uv) * vec4(1.0 - valueFire * (1.0 + uv.y), 1.0 - valueFire * (1.0 + uv.y), 1.0, 1.0);
    }
    color *= textureColor;
}
