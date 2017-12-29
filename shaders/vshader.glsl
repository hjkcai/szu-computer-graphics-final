#version 330 core

layout(location = 0) in vec3 vPosition;     // model space
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vNormal;       // model space

uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform vec3 LightPosition;     // world space

out vec2 UV;
out vec3 Position;        // world space
out vec3 Normal;          // camera space
out vec3 EyeDirection;    // camera space
out vec3 LightDirection;  // camera space

void main () {
  // 顶点位置
  gl_Position = MVP * vec4(vPosition, 1.0);

  // 计算世界坐标系中的顶点位置
  Position = (Model * vec4(vNormal, 1)).xyz;

	// 计算相机坐标系中，每个顶点到相机方向的向量
	// 注：在相机坐标系中，相机始终在原点上
  EyeDirection = vec3(0, 0, 0) - (View * Model * vec4(vPosition, 1)).xyz;

	// 计算每个顶点到光源的向量
	LightDirection = (View * vec4(LightPosition, 1)).xyz + EyeDirection;

	// 计算相机坐标系中的各个顶点的法向量
	Normal = (View * Model * vec4(vNormal, 0)).xyz;

	// 贴图位置
  UV = vertexUV;
}
