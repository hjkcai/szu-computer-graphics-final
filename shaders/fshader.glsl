#version 330 core

in vec2 UV;
in vec3 vPosition_world;
in vec3 vNormal_camera;
in vec3 eyeDirection;
in vec3 lightDirection;
in vec4 ShadowCoord;

uniform sampler2DShadow shadowMap;
uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 lightPosition;
uniform bool lightingEnabled;

out vec3 color;

vec2 poissonDisk[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

void main () {
	vec3 originalColor = texture(textureSampler, UV).rgb;
	// float visibility = texture(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z) / ShadowCoord.w));
	float bias = 0.005;
	float visibility = 1.0;
	// Sample the shadow map 4 times
	for (int i=0;i<4;i++){
		// use either :
		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
		int index = i;
		//  - A random sample, based on the pixel's screen location.
		//    No banding, but the shadow moves with the camera, which looks weird.
		// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		//  - A random sample, based on the pixel's position in world space.
		//    The position is rounded to the millimeter to avoid too much aliasing
		// int index = int(16.0*random(floor(Position_worldspace.xyz*1000.0), i))%16;

		// being fully in the shadow will eat up 4*0.2 = 0.8
		// 0.2 potentially remain, which is quite dark.
		visibility -= 0.2*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + poissonDisk[index]/700.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
	}

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
		vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
		vec3 specularColor = vec3(0.1, 0.1, 0.1);

		// 叠加三种光线并输出
		color =
			// 环境光
			ambientColor +
			// 漫反射
			visibility * diffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
			// 镜面反射
			visibility * specularColor * lightColor * lightPower * pow(cosAlpha, 20) / (distance * distance);
	} else {
		color = visibility * originalColor;
	}
}
