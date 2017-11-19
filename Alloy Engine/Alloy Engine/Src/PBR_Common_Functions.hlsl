
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
float GSmith(float _a, float _nrml_dot_vec, float _nrml_dot_light)
{
	return GGX(_nrml_dot_light, _a * _a) * GGX(_nrml_dot_vec, _a * _a);
}


/*
Fresenel: defines reflection, uses schlick approx.
http://blog.selfshadow.com/publications/s2013-shading-course/rad/s2013_pbs_rad_notes.pdf
*/
float3 SchlickFresnel(float3 _f, float3 _h, float3 _l)
{
	return _f + (1.0f - _f) * pow((1.0f - dot(_l, _h)), 5.0f);
}
