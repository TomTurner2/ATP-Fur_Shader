#include "Matrix.h"
#include <memory>


const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,  0.f, 0.f, 0.f, 1.f };


Matrix::Matrix()
	: _11(0.f), _12(0.f), _13(0.f), _14(0.f)
	, _21(0.f), _22(0.f), _23(0.f), _24(0.f)
	, _31(0.f), _32(0.f), _33(0.f), _34(0.f)
	, _41(0.f), _42(0.f), _43(0.f), _44(0.f)
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
