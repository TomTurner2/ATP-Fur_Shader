
//Constants
static const float Pi = 3.141592654f;


/*
Hammersley: Generates points on a sphere
http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
*/
float2 Hammersley(uint _i, uint _n)
{
	float radical_inverse = reversebits(_i) * 2.3283064365386963e-10f;
	return float2(float(_i) / float(_n), radical_inverse);
}


/*
CGX BRDF Specular
http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
*/
float GGX(float _nrml_dot_vec, float _a)
{
	float k = _a * 0.5f;
	return _nrml_dot_vec / (_nrml_dot_vec * (1.0f - k) + k);
}


/*
Geometry Term: shadowing of microfacets, smith approx.
http://blog.selfshadow.com/publications/s2013-shading-course/rad/s2013_pbs_rad_notes.pdf
*/
float G_Smith(float _a, float _nrml_dot_vec, float _nrml_dot_light)
{
	return GGX(_nrml_dot_light, _a * _a) * GGX(_nrml_dot_vec, _a * _a);
}


/*
Fresenel: defines reflection, uses schlick approx.
http://blog.selfshadow.com/publications/s2013-shading-course/rad/s2013_pbs_rad_notes.pdf
*/
float3 Schlick_Fresnel(float3 _f, float3 _h, float3 _l)
{
	return _f + (1.0f - _f) * pow((1.0f - dot(_l, _h)), 5.0f);
}


float3 ImportanceSampleGGX(float2 Xi, float _roughness, float3 _n)
{
	float a = _roughness * _roughness;

	float phi = 2 * Pi * Xi.x;
	float cos_theta = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
	float sin_theta = sqrt(1 - cos_theta * cos_theta);

	float3 h;
	h.x = sin_theta * cos(phi);
	h.y = sin_theta * sin(phi);
	h.z = cos_theta;

	float3 vector_up = abs(_n.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 tangent_x = normalize(cross(vector_up, _n));
	float3 tangent_y = cross(_n, tangent_x);

	return tangent_x * h.x + tangent_y * h.y + _n * h.z;//tangent world space
}