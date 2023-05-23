#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec4 textureColor;

void main() {
    color = texture(textureSampler, uv).rgba * textureColor;
}