struct FurLayer
{
	float4 position	: SV_POSITION;
	float4 world_position : POSITION;
	float4 normal   : NORMAL;
	int layer : LAYER;
};


struct VertexOut
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
};


void CreateShellVertex(inout TriangleStream<FurLayer> output_stream, float4 world_pos, float4 pos, float4 normal, int layer)
{
	FurLayer output;
	output.world_position = world_pos;
	output.position = pos;
	output.normal = normal;
	output.layer = layer;
	output_stream.Append(output);
}


[maxvertexcount(64)]
void main(triangle VertexOut input[3], inout TriangleStream<FurLayer> output_stream)
{
	for (float i = 0; i < 10; ++i)
	{
		CreateShellVertex(output_stream, input[0].world_position + (input[0].normal * i), input[0].position, input[0].normal, i);
		CreateShellVertex(output_stream, input[1].world_position + (input[1].normal * i), input[1].position, input[1].normal, i);
		CreateShellVertex(output_stream, input[2].world_position + (input[2].normal * i), input[2].position, input[2].normal, i);
		output_stream.RestartStrip();
	}
}