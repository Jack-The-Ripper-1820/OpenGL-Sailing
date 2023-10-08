#version 440

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 toCameraVec;

out vec4 color;

vec4 DirectionalLightSpacePos;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;

	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edgeAngle;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;


uniform Material material;

uniform vec3 eyePosition;

uniform mat4 directionalLightTransform;

uniform mat4 model;

uniform float moveFactor;


const float waveStrength = 0.02;

vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

float CalcPointShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPos - light.position;
	float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(eyePosition - FragPos);
	float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;
	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= omniShadowMaps[shadowIndex].farPlane;   // Undo mapping [0;1]
		if (currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
}

float CalcShadowFactor(vec4 DirectionalLightSpacePos)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;
	if (projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

vec4 CalcDirectionalLight(vec4 DirectionalLightSpacePos)
{
	float ShadowFactor = CalcShadowFactor(DirectionalLightSpacePos);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, ShadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	float shadowFactor = CalcPointShadowFactor(pLight, shadowIndex);

	vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);
	float attenuation = pLight.exponent * distance * distance +
		pLight.linear * distance +
		pLight.constant;

	return (color / attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i], i);
	}

	return totalColor;
}

void main()
{
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(FragPos, 1.0);

	vec4 finalColor = CalcDirectionalLight(DirectionalLightSpacePos);
	finalColor += CalcPointLights();

	vec2 distortion1 = (texture(theTexture, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(theTexture, vec2(-TexCoord.x + moveFactor, TexCoord.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	vec4 texColor = texture(theTexture, TexCoord);

	vec2 newTexCoord = TexCoord + totalDistortion;
	newTexCoord.x = clamp(newTexCoord.x, 0.001, 0.999);
	newTexCoord.y = clamp(newTexCoord.y, 0.001, 0.999);

	vec4 newTexColor = texture(theTexture, newTexCoord);

	//vec3 viewVec = normalize(toCameraVec);
	//float fresnelFactor = 0;//dot(viewVec, vec3(0, 1, 0));
	//fresnelFactor = pow(fresnelFactor, 5);

	color = newTexColor * vCol * finalColor;
}