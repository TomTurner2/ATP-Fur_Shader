#include "PBR_Lighting.hlsl"
#include "Base_PS_Buffers.hlsl"


// must be same as vs struct
struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


float4 main(VertexOut pin) : SV_TARGET
{
	float3 albedo_tex = m_albedo.Sample(m_sampler_state, pin.uv);
	float3 final_albedo = albedo_tex * mat_params.diff;

	float4 specular_tex = m_specular.Sample(m_sampler_state, pin.uv);
	float3 final_specular = float3(specular_tex.w * mat_params.spec, specular_tex.w * mat_params.spec,
	specular_tex.w * mat_params.spec);

	float4 roughness_tex = m_roughness.Sample(m_sampler_state, pin.uv);
	float final_roughness = roughness_tex.w * mat_params.rough;

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b) * light.intensity;

	float3 direct_lighting = DirectLighting(final_roughness, pin.normal, camera_pos,
	light_colour, light_pos, final_albedo, final_specular, pin.world_position);

	return float4(direct_lighting, 1);
}