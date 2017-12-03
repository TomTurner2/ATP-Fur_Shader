struct FurLayer
{
	float4 position	: SV_POSITION;
	float4 world_position : POSITION;
	float4 normal   : NORMAL;
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
};


void CreateShellVertex(inout TriangleStream<FurLayer> output_stream, float4 position, float4 normal, int layer)
{
	FurLayer output;	
	output.position = mul(position, m_model);
	output.world_position = mul(output.position, m_model);
	output.normal = mul(normal, m_model);
	output.layer = layer;
	output_stream.Append(output);
}


[maxvertexcount(64)]
void main(triangle VertexOut input[3], inout TriangleStream<FurLayer> output_stream)
{
	for (float i = 0; i < 5; ++i)
	{
		CreateShellVertex(output_stream, input[0].position + (input[0].normal * i), input[0].normal, i);
		CreateShellVertex(output_stream, input[1].position + (input[1].normal * i), input[1].normal, i);
		CreateShellVertex(output_stream, input[2].position + (input[2].normal * i), input[2].normal, i);
		output_stream.RestartStrip();
	}
} 