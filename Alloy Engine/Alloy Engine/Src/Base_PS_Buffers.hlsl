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
};


cbuffer PerScene : register(b1)
{
	Light light;
	float extra;
}


struct PBRMaterialParams
{
	float roughness;
	float3 mat_diffuse;
	float3 mat_specular;	
};


cbuffer PerObject : register(b2)
{
	PBRMaterialParams mat_params;
	float extra_two;
}

