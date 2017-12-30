#version 330 core

in vec2 UV;
in vec3 vPosition_world;
in vec3 vNormal_camera;
in vec3 eyeDirection;
in vec3 lightDirection;

uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 lightPosition;
uniform bool lightingEnabled;

out vec3 color;

void main () {
	vec3 originalColor = texture(textureSampler, UV).rgb;

	if (lightingEnabled) {
		// 光源与物体的距离，距离越远光照越弱
		float distance = length(lightPosition - vPosition_world);

		// 计算漫反射参数
		// 构造顶点法向量和光源方向的单位向量，并计算它们夹角的余弦值
		vec3 n = normalize(vNormal_camera);
		vec3 l = normalize(lightDirection);
		float cosTheta = clamp(dot(n, l), 0, 1);

		// 计算镜面反射参数
		// 根据顶点法向量和光源方向计算反射光向量
		// 再计算它和相机位置的夹角的余弦值
		vec3 e = normalize(eyeDirection);
		vec3 r = reflect(-l, n);
		float cosAlpha = clamp(dot(e, r), 0, 1);

		// 定义颜色
		vec3 diffuseColor = originalColor;
		vec3 ambientColor = vec3(0.3, 0.3, 0.3) * diffuseColor;
		vec3 specularColor = vec3(0.5, 0.5, 0.5);

		// 叠加三种光线并输出
		color =
			// 环境光
			ambientColor +
			// 漫反射
			diffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
			// 镜面反射
			specularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
	} else {
		color = originalColor;
	}
}
