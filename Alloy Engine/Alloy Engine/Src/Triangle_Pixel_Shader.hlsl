struct Input
{
	float4 position : SV_POSITION;
	float3 colour : COLOR;
};


float4 main(Input input) : SV_TARGET
{
	return float4(input.colour.r, input.colour.g, input.colour.b, 1);
}