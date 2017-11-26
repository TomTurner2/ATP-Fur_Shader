#include "PBR_Lighting.hlsl"
#include "Base_PS_Buffers.hlsl"


// must be same as vs struct
struct PixelIn
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
};


float4 main(PixelIn pin) : SV_TARGET
{
	float3 diffuse_albedo = float3(0.5f, 0.5f, 0.5f);
	float3 specular_albedo = float3(0.8f, 0.8f, 0.8f);
	float roughness = 0.1f;

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b);

	float3 direct_lighting = DirectLighting(roughness, pin.normal, camera_pos,
	light_colour, light_pos, diffuse_albedo, specular_albedo, pin.world_position);

	return float4(direct_lighting, 1);
}