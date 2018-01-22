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


cbuffer FurParameters : register(b2)
{
	float fur_mask_multiplier;
	float max_fur_length;
	float layer_step;
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


[maxvertexcount(64)]
void main(triangle VertexOut input[3], inout TriangleStream<FurLayer> output_stream)
{
	float4 _gravity = float4(gravity, 0);

	for (float i = 0; i < max_fur_length; i+= layer_step)
	{
		//float height = m_mask.Load(float3(input[0].uv, 0));
		CreateShellVertex(output_stream, input[0].position + (input[0].normal * i /** (1 - height)*/ +
		_gravity * pow(i, 2)), input[0].normal, input[0].uv, i);

		//float height_two = m_mask.Load(float3(input[1].uv, 0));
		CreateShellVertex(output_stream, input[1].position + (input[1].normal * i /** (1 - height_two)*/ +
		_gravity * pow(i, 2)), input[1].normal, input[1].uv, i);

		//float height_three = m_mask.Load(float3(input[2].uv, 0));
		CreateShellVertex(output_stream, input[2].position + (input[2].normal * i /** (1 - height_three)*/+
		_gravity *  pow(i, 2)), input[2].normal, input[2].uv, i);
		output_stream.RestartStrip();
	}
} 