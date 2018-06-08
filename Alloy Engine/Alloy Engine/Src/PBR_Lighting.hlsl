// Constants.
static const float Pi = 3.141592654f;


float GGX(float _nrml_dot_light, float _roughness_squared)
{
	float half_roughness_squared = _roughness_squared * 0.5f;
	return _nrml_dot_light / (_nrml_dot_light *
	(1.0f - half_roughness_squared) + half_roughness_squared);
}


// Geometry term.
float GSmith(float _roughness_squared, float _nrml_dot_vec, float _nrml_dot_light)
{
	return GGX(_nrml_dot_light, _roughness_squared) *
	GGX(_nrml_dot_vec, _roughness_squared);
}


// Fresnel term.
float3 SchlickFresnel(float3 _specular_albedo, float3 _half_vec, float3 _light_dir)
{
	return _specular_albedo + (1.0f - _specular_albedo) *
	pow((1.0f - dot(_light_dir, _half_vec)), 5.0f);
}



float3 DirectDiffuseBRDF(float3 _diffuse_albedo, float _normal_dot_light)
{
	return (_diffuse_albedo * _normal_dot_light) / Pi;
}


float TrowbridgeReitz(float _roughness_squared, float _nrml_dot_half_vec)
{
	return _roughness_squared / (Pi * pow(_nrml_dot_half_vec *
	_nrml_dot_half_vec * (_roughness_squared - 1) + 1, 2.0f));// Trowbridge-Reitz.
}


// Microfacet BRDF Cook Torrence.
float3 DirectSpecularBRDF(float3 _roughness, float3 _specular_albedo, float3 _position_world_space, float3 _normal_world_space, float3 _camera_pos, float3 _light_dir)
{
	float3 view_direction = normalize(_camera_pos - _position_world_space);
	float3 half_vec = normalize(view_direction + _light_dir);

	float nrml_dot_half_vec = saturate(dot(_normal_world_space, half_vec));
	float nrml_dot_light = saturate(dot(_normal_world_space, _light_dir));
	float nrml_dot_view = max(dot(_normal_world_space, view_direction), 0.0001f);

	float roughness_squared = _roughness * _roughness;

	float  d = TrowbridgeReitz(roughness_squared, nrml_dot_half_vec);// Distribution term.

	float3 f = SchlickFresnel(_specular_albedo, half_vec, _light_dir);// Fresnel term.

	float  g = GSmith(roughness_squared, nrml_dot_view, nrml_dot_light);// Geometry term.

	return d * f * g;
}


float3 DirectLighting(float _roughness, float3 _normal_world_space, float3 _camera_pos, float3 _light_colour, float3 _light_pos, float3 _diffuse_albedo,
	float3 _specular_albedo, float3 _pos_world_space)
{
	float3 lighting = 0.0f;

	float3 pixel_to_light = _light_pos - _pos_world_space;
	float  light_distance = length(pixel_to_light);
	float3 light_dir = pixel_to_light / light_distance;// Calc light direction.

	float nrml_dot_light = saturate(dot(_normal_world_space, light_dir));// Calc falloff.

	if (nrml_dot_light > 0.0f)
	{
		lighting = DirectDiffuseBRDF(_diffuse_albedo, nrml_dot_light) +
		DirectSpecularBRDF(_roughness, _specular_albedo, _pos_world_space, _normal_world_space, _camera_pos, light_dir);
	}

	return max(lighting, 0.0f) * _light_colour;
}