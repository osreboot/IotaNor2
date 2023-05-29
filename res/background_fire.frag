#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureBase, textureBase1;
uniform vec4 textureColor;

void main() {
    color = vec4(texture(textureBase, uv).r * texture(textureBase1, uv).r * -uv.y,
                 texture(textureBase, uv).g * texture(textureBase1, uv).g * -uv.y,
                 texture(textureBase, uv).b * texture(textureBase1, uv).b * -uv.y, 1.0);
}
