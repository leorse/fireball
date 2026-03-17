#version 330 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uHeightmap;
uniform sampler2D uPalette;
uniform sampler2D uCache;
uniform vec2      uTexelSize;
uniform vec2      uLightPos;
uniform float     uLightRadius;
uniform float     uPaletteOffset;
uniform float     uCalcOffset;   // = 1.0 / offset (offset in [1,20])

void main() {
    float h  = texture(uHeightmap, vUV).r * 255.0;
    float hL = texture(uHeightmap, vUV - vec2(uTexelSize.x, 0.0)).r * 255.0;
    float hD = texture(uHeightmap, vUV + vec2(0.0, uTexelSize.y)).r * 255.0;

    float xdelta = hL - h;
    float ydelta = h  - hD;

    float incX = vUV.x / uTexelSize.x;
    float incY = vUV.y / uTexelSize.y;

    // Bump mapping (same as light.frag)
    float dx = xdelta + incX - uLightPos.x;
    float dy = ydelta + incY - uLightPos.y;
    float dist = length(vec2(dx, dy));
    float bumpPixel = clamp(256.0 - dist * 256.0 / uLightRadius, 0.0, 255.0);

    // Shadow projection: find where this fragment's shadow falls
    float hauteurLumiereCache = 5.0 - uCalcOffset / 4.0;
    float hauteurCache        = 5.0;
    float rapportHauteur      = hauteurLumiereCache / (hauteurCache + hauteurLumiereCache);

    float longX = uLightPos.x - incX;
    float longY = uLightPos.y - incY;
    float posX  = uLightPos.x - longX * rapportHauteur - uCalcOffset;
    float posY  = uLightPos.y - longY * rapportHauteur - uCalcOffset;

    float cacheHere = texture(uCache, vUV).r;

    if (cacheHere > 0.0) {
        // Fragment is on the object: full brightness
        bumpPixel = 255.0;
    } else {
        // Check if this fragment lies in the projected shadow of the cache
        vec2 shadowUV = vec2(posX * uTexelSize.x, posY * uTexelSize.y);
        if (posX >= 0.0 && posY >= 0.0 &&
            posX < (1.0 / uTexelSize.x) && posY < (1.0 / uTexelSize.y)) {
            float cacheVal = texture(uCache, shadowUV).r * 255.0;
            if (cacheVal > 0.0) {
                float shadow = max(0.0, cacheVal - 100.0);
                bumpPixel = bumpPixel <= shadow ? 0.0 : bumpPixel - shadow;
            }
        }
    }

    float value = bumpPixel / 255.0;
    vec3 col = texture(uPalette, vec2(value, 0.5)).rgb;
    col = max(vec3(0.0), col - vec3(uPaletteOffset));
    fragColor = vec4(col, 1.0);
}
