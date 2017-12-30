#version 330 core

layout(location = 0) in vec3 vPosition;     // 模型坐标系
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vNormal;       // 模型坐标系

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;     // 世界坐标系
uniform mat4 DepthBiasMVP;

out vec2 UV;
out vec3 vPosition_world;       // 世界坐标系
out vec3 vNormal_camera;        // 相机坐标系
out vec3 eyeDirection;    			// 相机坐标系
out vec3 lightDirection;  			// 相机坐标系
out vec4 ShadowCoord;

void main () {
  // 顶点位置
  gl_Position = projection * view * model * vec4(vPosition, 1.0);

  // 计算世界坐标系中的顶点位置
  vPosition_world = (model * vec4(vNormal, 1)).xyz;

	// 计算相机坐标系中，每个顶点到相机方向的向量
	// 注：在相机坐标系中，相机始终在原点上
  eyeDirection = vec3(0, 0, 0) - (view * model * vec4(vPosition, 1)).xyz;

	// 计算每个顶点到光源的向量
	lightDirection = (view * vec4(lightPosition, 1)).xyz + eyeDirection;

	// 计算相机坐标系中的各个顶点的法向量
	vNormal_camera = (view * model * vec4(vNormal, 0)).xyz;

	// 贴图位置
  UV = vertexUV;

	ShadowCoord = DepthBiasMVP * vec4(vPosition, 1);
}
