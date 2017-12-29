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

  // Position of the vertex, in worldspace : M * position
  Position = (Model * vec4(vNormal, 1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
  EyeDirection = vec3(0, 0, 0) - (View * Model * vec4(vPosition, 1)).xyz;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	LightDirection = (View * vec4(LightPosition, 1)).xyz + EyeDirection;

	// Normal of the the vertex, in camera space
	Normal = (View * Model * vec4(vNormal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	// UV of the vertex. No special space for this one.
  UV = vertexUV;
}
