#version 330 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uHeightmap;
uniform sampler2D uPalette;
uniform sampler2D uCache;
uniform vec2      uTexelSize;

void main() {
    float h  = texture(uHeightmap, vUV).r * 255.0;
    float hL = texture(uHeightmap, vUV - vec2(uTexelSize.x, 0.0)).r * 255.0;
    float hD = texture(uHeightmap, vUV + vec2(0.0, uTexelSize.y)).r * 255.0;

    // Bump gradient → refraction offset (matches original xdelta/ydelta)
    float xdelta = hL - h;
    float ydelta = h  - hD;

    // Displaced UV into cache (replicates xtemp = incX - xdelta, ytemp = incY - ydelta)
    vec2 refractedUV = vUV - vec2(xdelta, ydelta) * uTexelSize;
    refractedUV = clamp(refractedUV, vec2(0.0), vec2(1.0));

    float cacheVal = texture(uCache, refractedUV).r;

    // Full palette lookup (no offset for glass mode)
    fragColor = vec4(texture(uPalette, vec2(cacheVal, 0.5)).rgb, 1.0);
}
