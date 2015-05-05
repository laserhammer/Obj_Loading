#version 440

const int MAX_LIGHTS = 8;

struct Light
{
	vec4 position;
	vec4 color_power;
	vec4 ambient;
};

in vertToFrag
{
	vec4 Color;
	vec4 Normal;
	vec4 WorldPos;
	vec4 CamPos;
};

layout (std140) uniform lightsBlock
{
	Light lights[MAX_LIGHTS];
};

out vec4 outColor;

void main()
{
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		vec3 lightDir = lights[i].position.xyz - WorldPos.xyz;
		float dis = length(lightDir);
		
		float NdotL = dot(vec4(-lightDir.xyz, 0.0), Normal);
		float intensity = clamp(NdotL, 0.0, 1.0);
		diffuse += intensity * vec4(lights[i].color_power.xyz, 1.0) * lights[i].color_power.w / (dis * dis) + lights[i].ambient;
		
		vec4 highlight = -reflect(vec4(lightDir.xyz, 0.0), Normal);
		vec4 outVec = normalize(CamPos) - normalize(WorldPos);
		float NdotOut = clamp(dot(vec4(-lightDir.xyz, 0.0), outVec), 0.0, 1.0);
		vec4 specIntensity = vec4(lights[i].color_power.xyz, 1.0) * pow(max(dot(highlight, outVec), 0.0), 0.3) * NdotOut * NdotL;
		specular += clamp(specIntensity, 0.0, 1.0);
	}
	
	outColor = specular + diffuse * Color;
};