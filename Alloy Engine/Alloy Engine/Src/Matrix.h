#pragma once

class Vector3;

class Matrix
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		float m[4][4];
	};

	Matrix();
	Matrix(const Matrix& _matrix) = default;
	Matrix(const float* _array);
	Matrix(float _m_11, float _m_12, float _m_13, float _m_14, float _m_21, float _m_22,
		float _m_23, float _m_24, float _m_31, float _m_32, float _m_33, float _m_34,
		float _m_41, float _m_42, float _m_43, float _m_44);
	~Matrix() = default;

	Matrix Transpose() const;
	float Determinant() const;

	Matrix operator*(const Matrix& _matrix) const;

	static Matrix LookAt(const Vector3& _pos,  Vector3& _look_target, const Vector3& _up);
	static Matrix PerspectiveProjection(float _fov, float _aspect, float _near_clip, float _far_clip);
	
	static const Matrix Identity;
};
