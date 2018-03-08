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


cbuffer FurParameters : register(b3)
{
	float fur_mask_multiplier;
	float max_fur_length;
	int layer_count;
	float base_clip;
	float shadow_intensity;
	float3 gravity;
}


Texture2D m_alpha : register(t3);
Texture2D m_mask : register(t4);


float4 main(FurLayer pin) : SV_TARGET
{	
	float3 alpha_mask = m_alpha.Sample(m_sampler_state, pin.uv);// Sample fur strand alpha.
	alpha_mask *= m_mask.Sample(m_sampler_state, pin.uv);// Apply fur height mask.
	alpha_mask = saturate(alpha_mask);// Normalise.

	float alpha = smoothstep(base_clip, alpha_mask.x, 0.5f);// Clip the strands.
	alpha = lerp(1, alpha, max(pin.layer, 0));// Replaces if statement below (optimisation).
	//if (pin.layer <= 0)// First layer should have no transparency.
	//	alpha = 1;

	float threshold = 0.5f * pin.layer * base_clip;
	if (alpha <= threshold )// If below threshold discard the pixel.
		discard;

	// Calculate after the discard, no point calculating colour for unwanted pixel (optimisation).
	float3 albedo_tex = m_albedo.Sample(m_sampler_state, pin.uv);
	float3 final_albedo = albedo_tex * mat_params.diff;

	float4 specular_tex = m_specular.Sample(m_sampler_state, pin.uv);
	float3 final_specular = float3(specular_tex.a, specular_tex.a, specular_tex.a) * mat_params.spec;// Spec is stored in alpha.

	float4 roughness_tex = m_roughness.Sample(m_sampler_state, pin.uv);
	float final_roughness = roughness_tex.r * mat_params.rough;// Roughness stored in r channel.

	float3 light_pos = light.position;
	float3 light_colour = float3(light.r, light.g, light.b) * light.intensity;

	float3 direct_lighting = DirectLighting(final_roughness, pin.normal, camera_pos,
	light_colour, light_pos, final_albedo, final_specular, pin.world_position);

	float shadow = lerp(shadow_intensity, 1, pin.layer);// Fake fur shadowing.
	shadow = saturate(shadow);

	return float4(direct_lighting * shadow, alpha);
}