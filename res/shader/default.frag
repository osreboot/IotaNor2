#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase;
uniform vec4 textureColor;

void main() {
    color = texture(textureBase, uv).rgba * textureColor;
}