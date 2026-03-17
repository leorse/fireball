#version 330 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uHeightmap;
uniform sampler2D uPalette;
uniform vec2      uTexelSize;    // (1/width, 1/height)
uniform vec2      uLightPos;     // light position in pixels
uniform float     uLightRadius;  // min(width,height)/2 in pixels
uniform float     uPaletteOffset; // 0..1, subtracted from palette RGB

void main() {
    float h  = texture(uHeightmap, vUV).r * 255.0;
    float hL = texture(uHeightmap, vUV - vec2(uTexelSize.x, 0.0)).r * 255.0;
    float hD = texture(uHeightmap, vUV + vec2(0.0, uTexelSize.y)).r * 255.0;

    // Normal from bump gradient (matches original xdelta/ydelta)
    float xdelta = hL - h;
    float ydelta = h  - hD;

    // Pixel coordinates of current fragment
    float incX = vUV.x / uTexelSize.x;
    float incY = vUV.y / uTexelSize.y;

    // Displaced position relative to light (replicates lx/ly + phongmap lookup)
    float dx = xdelta + incX - uLightPos.x;
    float dy = ydelta + incY - uLightPos.y;
    float dist = length(vec2(dx, dy));

    float value = clamp((256.0 - dist * 256.0 / uLightRadius) / 255.0, 0.0, 1.0);

    vec3 col = texture(uPalette, vec2(value, 0.5)).rgb;
    col = max(vec3(0.0), col - vec3(uPaletteOffset));
    fragColor = vec4(col, 1.0);
}
