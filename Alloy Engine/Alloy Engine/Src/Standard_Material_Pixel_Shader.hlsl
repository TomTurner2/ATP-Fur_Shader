#include "PBR_Lighting.hlsl"


cbuffer PerFrame : register(b0)
{
	float4 camera_pos;//might need to do the 1 and 0 thing in the vertex
}


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

	float3 light_pos = float3(50.0f, 50.0f, -250.0f);
	float3 light_colour = float3(1, 1, 1);

	float3 direct_lighting = DirectLighting(roughness, pin.normal, camera_pos,
	light_colour, light_pos, diffuse_albedo, specular_albedo, pin.world_position);

	return float4(direct_lighting, 1);
}