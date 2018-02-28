struct FurLayer
{
	float4 position	: SV_POSITION;
	float4 world_position : POSITION;
	float4 normal   : NORMAL;
	float2 uv : TEXCOORD0;
	int layer : LAYER;
};


cbuffer PerObject : register(b0)
{
	matrix m_model;
};


cbuffer PerFrame : register(b1)
{
	matrix m_view;
	matrix m_proj;
};


struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


cbuffer FurParameters : register(b2)
{
	float fur_mask_multiplier;
	float max_fur_length;
	int layer_count;
	float base_clip;
	float end_clip;
	float3 gravity;
}


Texture2D m_alpha : register(t0);
Texture2D m_mask : register(t1);


void CreateShellVertex(inout TriangleStream<FurLayer> _output_stream, float4 _position, float4 _normal, float2 _uv, float _layer)
{
	FurLayer output;	
	output.position = mul(_position, m_model);
	output.world_position = mul(output.position, m_model);
	output.normal = mul(_normal, m_model);
	output.layer = _layer;
	output.uv = _uv;
	_output_stream.Append(output);
}


float4 CalculatedGravity(float _layer_offset)
{
	float gravity_factor = pow(_layer_offset, 3);//make it bend the further out the layer
	return float4(gravity.xyz, 0) * gravity_factor;//multiply it by the gravity direction
}


float4 CalculateShellOffset(VertexOut _vert, float _layer_offset)
{
	float height = m_mask.Load(float3(_vert.uv, 0));//load height mask
	return(_vert.position + _vert.normal * _layer_offset * 1 - height) + CalculatedGravity(_layer_offset);//offset by surface normal, height, and gravity
}


[maxvertexcount(64)]
void main(triangle VertexOut input[3], inout TriangleStream<FurLayer> output_stream)
{
	float step = max_fur_length / layer_count;//based on layer count and max length calculate step size

	for (float i = 0; i < max_fur_length; i+= step)
	{
		//Shell the three vertices of the triangle
		CreateShellVertex(output_stream, CalculateShellOffset(input[0], i), input[0].normal, input[0].uv, i);
		CreateShellVertex(output_stream, CalculateShellOffset(input[1], i), input[1].normal, input[1].uv, i);
		CreateShellVertex(output_stream, CalculateShellOffset(input[2], i), input[2].normal, input[2].uv, i);

		output_stream.RestartStrip();
	}
} 