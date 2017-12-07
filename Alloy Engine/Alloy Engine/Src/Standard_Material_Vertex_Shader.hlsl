#include "Base_VS_Buffers.hlsl"

//pixel that is output
struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


VertexOut main(VertexIn vin)
{
	VertexOut vout;

	vout.position = mul(vin.position, m_model);//convert to object space

	//place in front of camera
	vout.position = mul(vout.position, m_view);
	vout.position = mul(vout.position, m_proj);

	vout.world_position = mul(vin.position, m_model);//convert to world space
	vout.normal = mul(vin.normal, m_model);//convert to world space normals
	vout.uv = vin.uv;

	return vout;
}