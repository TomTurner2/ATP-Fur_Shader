#include "PBR_Lighting.hlsl"
#include "Base_PS_Buffers.hlsl"


// Must be same as vs struct.
struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


float4 main(VertexOut pin) : SV_TARGET
{
	float3 final_albedo = mat_params.diff;
	float3 final_specular = float3(mat_params.spec, mat_params.spec, mat_params.spec);
	float final_roughness = mat_params.rough;

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b) * light.intensity;

	float3 direct_lighting = DirectLighting(final_roughness, pin.normal, camera_pos,
	light_colour, light_pos, final_albedo, final_specular, pin.world_position);

	return float4(direct_lighting, 1);
}