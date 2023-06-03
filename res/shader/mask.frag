#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase, textureChannel0, textureChannel1;
uniform vec4 textureColor;

void main() {
    float interp = texture(textureBase, uv).r;
    color = (((1.0 - interp) * texture(textureChannel0, uv)) + (interp * texture(textureChannel1, uv))) * textureColor;
}
