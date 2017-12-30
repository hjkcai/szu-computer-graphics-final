#version 330 core

// 输出每个片源与光源之间的距离
layout(location = 0) out float fragmentDepth;

void main () {
  fragmentDepth = gl_FragCoord.z;
}
