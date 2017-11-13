cbuffer PerFrame : register(b0)
{
	float4 camera_pos;//might need to do the 1 and 0 thing in the vertex
}


// must be same as vs struct
struct PixelIn
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;
	float4 normal : NORMAL;
};


float4 main(PixelIn pin) : SV_TARGET
{
	float4 diffuse = float4(1.f, 0.f, 0.f, 1.f);

	return saturate(diffuse);
}