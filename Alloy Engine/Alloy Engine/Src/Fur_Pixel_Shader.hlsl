#include "PBR_Lighting.hlsl"
#include "Base_PS_Buffers.hlsl"


struct FurLayer
{
	float4 position	: SV_POSITION;
	float4 world_position : POSITION;
	float4 normal   : NORMAL;
	float2 uv : TEXCOORD0;
	int layer : LAYER;
};


float4 main(FurLayer pin) : SV_TARGET
{
	float3 diffuse_albedo = float3(mat_params.diff[0], mat_params.diff[1], mat_params.diff[2]);//get material parameters
	float3 specular_albedo = float3(mat_params.spec, mat_params.spec, mat_params.spec);
	float roughness = mat_params.rough;

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b);

	float3 direct_lighting = DirectLighting(roughness, pin.normal, camera_pos,
	light_colour, light_pos, diffuse_albedo, specular_albedo, pin.world_position);//calculate lighting
	
	float alpha = 1 - (pin.layer / 5);//get alpha according to shell layer

	if (alpha < 0.05f)//if zero discard the pixel
		discard;

	return float4(direct_lighting, alpha);
}