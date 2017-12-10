//this file contains all the base pixel shader constant buffers

cbuffer PerFrame : register(b0)
{
	float4 camera_pos;
}


struct Light
{
	float3 position;
	float r;
	float g;
	float b;
	float intensity;
};


cbuffer PerScene : register(b1)
{
	Light light;
	float extra;
}


struct PBRMaterialParams
{
	float3 diff;
	float rough;
	float spec;
};


cbuffer PerObject : register(b2)
{
	PBRMaterialParams mat_params;
	float extra_two;
}


//default texture maps
SamplerState m_sampler_state : register(s0);
Texture2D m_albedo : register(t0);
Texture2D m_roughness : register(t1);
Texture2D m_specular : register(t2);
