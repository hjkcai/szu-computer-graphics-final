#version 330 core

layout(location = 0) in vec3 vPosition;   // 模型坐标系

uniform mat4 model;           // 模型变换
uniform mat4 view;            // 旋转整个视图，使得在相机坐标系中，光照方向为 -Z
uniform mat4 projection;      // 一个包含整个绘制场景的投影矩阵

void main () {
  mat4 depthMVP = projection * view * model;
  gl_Position = depthMVP * vec4(vPosition, 1);
}
