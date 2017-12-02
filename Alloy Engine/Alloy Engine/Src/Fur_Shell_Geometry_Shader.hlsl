

struct VertexIn
{
	float4 position : POSITION;
	float4 normal : NORMAL;
};


struct FurLayer
{
	float4 position	: SV_Position;
	float3 normal   : NORMAL;
	int layer : LAYER;
};


[maxvertexcount(6)]
void main(point VertexIn input[1], inout PointStream<FurLayer> output_stream )
{
	FurLayer output;

	int max_length = 5;
	for (int i = 1; i < max_length + 1; ++i)
	{
		float3 shell_pos = input[0].position.xyz + (input[0].normal * i);
		output.position = float4(shell_pos.x, shell_pos.y, shell_pos.z, 0);
		output.normal = input[0].normal;
		output.layer = i;
		output_stream.Append(output);
	}
}