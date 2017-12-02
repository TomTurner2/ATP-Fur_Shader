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
	float3 diffuse_albedo = float3(mat_params.diff[0], mat_params.diff[1], mat_params.diff[2]);
	float3 specular_albedo = float3(mat_params.spec, mat_params.spec, mat_params.spec);
	float roughness = mat_params.rough;

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b);

	float3 direct_lighting = DirectLighting(roughness, pin.normal, camera_pos,
	light_colour, light_pos, diffuse_albedo, specular_albedo, pin.world_position);

	return float4(direct_lighting, 1);
}