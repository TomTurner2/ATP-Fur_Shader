#include "Matrix.h"
#include <memory>
#include "Vector3.h"


const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,  0.f, 0.f, 0.f, 1.f };


Matrix::Matrix()
	: _11(0.f), _12(0.f), _13(0.f), _14(0.f)
	, _21(0.f), _22(0.f), _23(0.f), _24(0.f)
	, _31(0.f), _32(0.f), _33(0.f), _34(0.f)
	, _41(0.f), _42(0.f), _43(0.f), _44(0.f)
{}


Matrix::Matrix(const float* _array) : _11(_array[0]), _12(_array[1]), _13(_array[2]), _14(_array[3])
, _21(_array[4]), _22(_array[5]), _23(_array[6]), _24(_array[7])
, _31(_array[8]), _32(_array[9]), _33(_array[10]), _34(_array[11])
, _41(_array[12]), _42(_array[13]), _43(_array[14]), _44(_array[15])
{}


Matrix::Matrix(float _m_11, float _m_12, float _m_13, float _m_14, float _m_21,
	float _m_22, float _m_23, float _m_24, float _m_31, float _m_32, float _m_33, float _m_34,
	float _m_41, float _m_42, float _m_43, float _m_44)
	: _11(_m_11), _12(_m_12), _13(_m_13), _14(_m_14)
	, _21(_m_21), _22(_m_22), _23(_m_23), _24(_m_24)
	, _31(_m_31), _32(_m_32), _33(_m_33), _34(_m_34)
	, _41(_m_41), _42(_m_42), _43(_m_43), _44(_m_44)
{}


Matrix Matrix::Transpose() const
{
	Matrix transpose;

	for (unsigned int row = 0; row < 4; ++row)
	{
		for (unsigned int column = 0; column < 4; ++column)
		{
			transpose.m[row][column] = m[column][row];
		}
	}

	return transpose;
}


float Matrix::Determinant() const
{
	return
		_11 * (_22 * _33 - _23 * _32)
		- _12 * (_21 * _33 - _13 * _31)
		+ _13 * (_21 * _32 - _23 * _31);
}


Matrix Matrix::operator*(const Matrix& _matrix) const
{
	Matrix mult_matrix;

	for (unsigned int row = 0; row < 4; ++row)
	{
		for (unsigned int col = 0; col < 4; ++col)
		{
			mult_matrix.m[row][col] = 0.f;
			for (unsigned int k = 0; k < 4; ++k)
			{
				mult_matrix.m[row][col] += m[row][k] * _matrix.m[k][col];
			}
		}
	}

	return std::move(mult_matrix);
}


Matrix Matrix::LookAt(const Vector3& _pos, Vector3& _look_target, const Vector3& _up)
{
	Vector3 z_axis = (_look_target - _pos).Normalised();
	Vector3 x_axis = Vector3::Cross(_up, z_axis).Normalised();
	Vector3 y_axis = Vector3::Cross(z_axis, x_axis);

	return Matrix(
		x_axis.x, y_axis.x, z_axis.x, 0.f,
		x_axis.y, y_axis.y, z_axis.y, 0.f,
		x_axis.z, y_axis.z, z_axis.z, 0.f,
		-Vector3::Dot(x_axis, _pos), -Vector3::Dot(y_axis, _pos), -Vector3::Dot(z_axis, _pos), 1.f);
}


Matrix Matrix::PerspectiveProjection(float _fov, float _aspect, float _near_clip, float _far_clip)
{
	float y_scale = cosf(_fov / 2.f) / sinf(_fov / 2.f);
	float x_scale = y_scale / _aspect;

	return Matrix(
		x_scale, 0.f, 0.f, 0.f,
		0.f, y_scale, 0.f, 0.f,
		0.f, 0.f, _far_clip / (_far_clip - _near_clip), 1.f,
		0.f, 0.f, -_near_clip * _far_clip / (_far_clip - _near_clip), 0.f
	);
}
