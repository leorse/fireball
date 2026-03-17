#version 330 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uHeightmap;
uniform sampler2D uPalette;

void main() {
    float h = texture(uHeightmap, vUV).r;
    fragColor = vec4(texture(uPalette, vec2(h, 0.5)).rgb, 1.0);
}
