//this file contains all the base vertex shader buffers

//input
struct VertexIn
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


//input buffers
cbuffer PerObject : register(b0)
{
	matrix m_model;
};


cbuffer PerFrame : register(b1)
{
	matrix m_view;
	matrix m_proj;
};
