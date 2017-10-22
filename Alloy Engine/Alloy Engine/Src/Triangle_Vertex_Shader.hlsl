struct Input
{
	float2 position : POSITION;// 2D position
	float3 colour : COLOR;
};


struct Output
{
	float4 position : SV_POSITION;// Must be float 4 (x, y, z, w)
	float3 colour : COLOR;
};


Output main(Input input)
{
	Output output;

	output.position = float4(input.position.x, input.position.y, 0, 1);
	output.colour = input.colour;

	return output;
}