#version 330 core

in vec2 UV;
in vec3 Position;
in vec3 Normal;
in vec3 EyeDirection;
in vec3 LightDirection;

uniform sampler2D textureSampler;
uniform vec3 LightPosition;

out vec3 color;

void main () {
  // TODO: make uniform
  vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 50.0f;

	// Distance to the light
	float distance = length(LightPosition - Position);

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(Normal);

	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection);

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection);

	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

  // Material color
  vec3 MaterialDiffuseColor = texture(textureSampler, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.3, 0.3, 0.3) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.5, 0.5, 0.5);

  // Output color
	color =
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5) / (distance * distance);
}
