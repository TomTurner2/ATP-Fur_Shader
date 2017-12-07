struct FurLayer
{
	float4 position	: SV_POSITION;
	float4 world_position : POSITION;
	float4 normal   : NORMAL;
	float2 uv : TEXCOORD0;
	int layer : LAYER;
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


struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
};


void CreateShellVertex(inout TriangleStream<FurLayer> _output_stream, float4 _position, float4 _normal, float2 _uv, int _layer)
{
	FurLayer output;	
	output.position = mul(_position, m_model);
	output.world_position = mul(output.position, m_model);
	output.normal = mul(_normal, m_model);
	output.layer = _layer;
	output.uv = _uv;
	_output_stream.Append(output);
}


[maxvertexcount(64)]
void main(triangle VertexOut input[3], inout TriangleStream<FurLayer> output_stream)
{
	for (float i = 0; i < 5; ++i)
	{
		CreateShellVertex(output_stream, input[0].position + (input[0].normal * i), input[0].normal, input[0].uv, i);
		CreateShellVertex(output_stream, input[1].position + (input[1].normal * i), input[1].normal, input[1].uv, i);
		CreateShellVertex(output_stream, input[2].position + (input[2].normal * i), input[2].normal, input[2].uv, i);
		output_stream.RestartStrip();
	}
} 