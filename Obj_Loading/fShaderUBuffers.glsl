#version 440

const int MAX_LIGHTS = 8;

struct Light
{
	vec4 position;
	vec4 color_power;
	vec4 ambient;
};

in vertexData
{
	vec4 Color;
	vec4 Normal;
	vec4 WorldPos;
	vec4 CamPos;
} ;

layout (std140) uniform lightsBlock
{
	Light lights[MAX_LIGHTS];
} ;

out vec4 outColor;

void main()
{
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	Light light;
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		light = lights[i];
		vec3 lightDir = light.position.xyz - WorldPos.xyz;
		float dis = length(lightDir);
		
		float NdotL = dot(vec4(-lightDir.xyz, 0.0), Normal);
		float intensity = clamp(NdotL, 0.0, 1.0);
		diffuse += intensity * light.color_power * light.color_power.w / (dis * dis) + light.ambient;
		
		vec4 highlight = -reflect(vec4(lightDir.xyz, 0.0), Normal);
		vec4 outVec = normalize(CamPos) - normalize(WorldPos);
		float specIntensity = pow(max(dot(highlight, outVec), 0.0), 0.3);
		specular += clamp(specIntensity * light.color_power, 0.0, 1.0) * intensity;
	}
	
	outColor = vec4(1.0, 1.0, 1.0, 1.0);//specular + diffuse * Color;
};