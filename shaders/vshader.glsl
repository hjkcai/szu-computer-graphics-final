#version 330 core

in vec3 vPosition;
in vec2 vUV;
uniform mat4 mvp;

out vec2 UV;

void main () {
  gl_Position = mvp * vec4(vPosition, 1.0);
  UV = vertexUV;
}
