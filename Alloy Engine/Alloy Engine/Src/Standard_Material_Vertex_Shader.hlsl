//input
struct VertexIn
{
	float4 position : POSITION;
	float4 normal : NORMAL;
};


//pixel that is output
struct PixelIn
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
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


PixelIn main(VertexIn vin)
{
	PixelIn vout;

	vout.position = mul(vin.position, m_model);//convert to object space

	//place in front of camera
	vout.position = mul(vout.position, m_view);
	vout.position = mul(vout.position, m_proj);

	vout.world_position = mul(vin.position, m_model);//convert to world space

	vout.normal = mul(vin.normal, m_model);//convert to world space normals
	return vout;
}