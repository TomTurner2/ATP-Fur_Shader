//Constants
static const float Pi = 3.141592654f;


float GGX(float _nrml_dot_vec, float _a)
{
	float k = _a * 0.5f;
	return _nrml_dot_vec / (_nrml_dot_vec * (1.0f - k) + k);
}


//Geometry term
float GSmith(float _a, float _nrml_dot_vec, float _nrml_dot_light)
{
	return GGX(_nrml_dot_light, _a * _a) * GGX(_nrml_dot_vec, _a * _a);
}


//Fresnel term
float3 SchlickFresnel(float3 _f, float3 _h, float3 _l)
{
	return _f + (1.0f - _f) * pow((1.0f - dot(_l, _h)), 5.0f);
}



float3 DirectDiffuseBRDF(float3 _diffuse_albedo, float _normal_dot_light)
{
	return (_diffuse_albedo * _normal_dot_light) / Pi;
}


//Microfacet BRDF Cook Torrence
float3 DirectSpecularBRDF(float3 _roughness, float3 _specular_albedo, float3 _position_world_space, float3 _normal_world_space, float3 _camera_pos, float3 _light_dir)
{
	float3 view_direction = normalize(_camera_pos - _position_world_space);
	float3 half_vec = normalize(view_direction + _light_dir);

	float nrml_dot_half_vec = saturate(dot(_normal_world_space, half_vec));
	float nrml_dot_light = saturate(dot(_normal_world_space, _light_dir));
	float nrml_dot_view = max(dot(_normal_world_space, view_direction), 0.0001f);

	float alpha_squared = _roughness * _roughness;

	float  d = alpha_squared / (Pi * pow(nrml_dot_half_vec * nrml_dot_half_vec * (alpha_squared - 1) + 1, 2.0f));//beckmann distribution

	float3 f = SchlickFresnel(_specular_albedo, half_vec, _light_dir);//fresnel

	float  g = GSmith(_roughness, nrml_dot_view, nrml_dot_light);//geometry term

	return d * f * g;
}


float3 DirectLighting(float _roughness, float3 _normal_world_space, float3 _camera_pos, float3 _light_colour, float3 _light_pos, float3 _diffuse_albedo,
	float3 _specular_albedo, float3 _pos_world_space)
{
	float3 lighting = 0.0f;

	float3 pixel_to_light = _light_pos - _pos_world_space;
	float  light_distance = length(pixel_to_light);
	float3 light_dir = pixel_to_light / light_distance;//calc light direction

	float nrml_dot_light = saturate(dot(_normal_world_space, light_dir));//calc falloff

	if (nrml_dot_light > 0.0f)
	{
		lighting = DirectDiffuseBRDF(_diffuse_albedo, nrml_dot_light) +
		DirectSpecularBRDF(_roughness, _specular_albedo, _pos_world_space, _normal_world_space, _camera_pos, light_dir);
	}

	return max(lighting, 0.0f) * _light_colour;
}