#version 330 core

layout(location = 0) in vec2 aPos;
out vec2 vUV;

void main() {
    // Flip Y: UV(0,0) = coin haut-gauche (convention SDL/screen)
    vUV = vec2(aPos.x * 0.5 + 0.5, 0.5 - aPos.y * 0.5);
    gl_Position = vec4(aPos, 0.0, 1.0);
}
