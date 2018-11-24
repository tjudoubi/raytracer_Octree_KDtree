#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector3D.h"

struct Matrix4x4
{
	union
	{
		double d[16];
		double m[4][4];
		struct
		{
			double _11, _12, _13, _14;
			double _21, _22, _23, _24;
			double _31, _32, _33, _34;
			double _41, _42, _43, _44;
		};
	};

	double& operator () ( int r, int c ) { return m[r][c]; }
	double operator () ( int r, int c ) const { return m[r][c]; }
	double& operator [] ( int idx ) { return d[idx]; }
	double operator [] ( int idx ) const { return d[idx]; }
	void Identity()
	{
	//	ZeroMemory( d, sizeof(double) * 16 );
		_11 = 1.0, _12 = 0.0, _13 = 0.0, _14 = 0.0;
		_21 = 0.0, _22 = 1.0, _23 = 0.0, _24 = 0.0;
		_31 = 0.0, _32 = 0.0, _33 = 1.0, _34 = 0.0;
		_41 = 0.0, _42 = 0.0, _43 = 0.0, _44 = 1.0;
	}
	void operator += ( const Matrix4x4 &rm ) 
	{
		d[0] += rm[0];  d[1] += rm[1];  d[2] += rm[2];  d[3] += rm[3];  
		d[4] += rm[4];  d[5] += rm[5];  d[6] += rm[6];  d[7] += rm[7];
		d[8] += rm[8];  d[9] += rm[9];  d[10] += rm[10];  d[11] += rm[11];  
		d[12] += rm[12];  d[13] += rm[13];  d[14] += rm[14];  d[15] += rm[15];
	}
	void operator -= ( const Matrix4x4 &rm ) 
	{ 
		d[0] -= rm[0];  d[1] -= rm[1];  d[2] -= rm[2];  d[3] -= rm[3];  
		d[4] -= rm[4];  d[5] -= rm[5];  d[6] -= rm[6];  d[7] -= rm[7];
		d[8] -= rm[8];  d[9] -= rm[9];  d[10] -= rm[10];  d[11] -= rm[11];  
		d[12] -= rm[12];  d[13] -= rm[13];  d[14] -= rm[14];  d[15] -= rm[15];
	}
	void operator *= ( float f )
	{
		d[0] *= f;  d[1] *= f;  d[2] *= f;  d[3] *= f;  
		d[4] *= f;  d[5] *= f;  d[6] *= f;  d[7] *= f;
		d[8] *= f;  d[9] *= f;  d[10] *= f;  d[11] *= f;  
		d[12] *= f;  d[13] *= f;  d[14] *= f;  d[15] *= f;
	}
	void operator /= ( float f )
	{
		d[0] /= f;  d[1] /= f;  d[2] /= f;  d[3] /= f;  
		d[4] /= f;  d[5] /= f;  d[6] /= f;  d[7] /= f;
		d[8] /= f;  d[9] /= f;  d[10] /= f;  d[11] /= f;  
		d[12] /= f;  d[13] /= f;  d[14] /= f;  d[15] /= f;
	}
	Matrix4x4 operator + ( const Matrix4x4 &rm ) const
	{
		Matrix4x4 ret;
		ret[0] = d[0] + rm[0];  ret[1] = d[1] + rm[1];  ret[2] = d[2] + rm[2];  ret[3] = d[3] + rm[3];  
		ret[4] = d[4] + rm[4];  ret[5] = d[5] + rm[5];  ret[6] = d[6] + rm[6];  ret[7] = d[7] + rm[7];
		ret[8] = d[8] + rm[8];  ret[9] = d[9] + rm[9];  ret[10] = d[10] + rm[10];  ret[11] = d[11] + rm[11];  
		ret[12] = d[12] + rm[12];  ret[13] = d[13] + rm[13];  ret[14] = d[14] + rm[14];  ret[15] = d[15] + rm[15];
		return ret;

	}
	Matrix4x4 operator - ( const Matrix4x4 &rm ) const
	{
		Matrix4x4 ret;
		ret[0] = d[0] - rm[0];  ret[1] = d[1] - rm[1];  ret[2] = d[2] - rm[2];  ret[3] = d[3] - rm[3];  
		ret[4] = d[4] - rm[4];  ret[5] = d[5] - rm[5];  ret[6] = d[6] - rm[6];  ret[7] = d[7] - rm[7];
		ret[8] = d[8] - rm[8];  ret[9] = d[9] - rm[9];  ret[10] = d[10] - rm[10];  ret[11] = d[11] - rm[11];  
		ret[12] = d[12] - rm[12];  ret[13] = d[13] - rm[13];  ret[14] = d[14] - rm[14];  ret[15] = d[15] - rm[15];
		return ret;
	}
	Matrix4x4 operator * ( float f ) const
	{
		Matrix4x4 ret;
		ret[0] = d[0] * f;  ret[1] = d[1] * f;  ret[2] = d[2] * f;  ret[3] = d[3] * f;  
		ret[4] = d[4] * f;  ret[5] = d[5] * f;  ret[6] = d[6] * f;  ret[7] = d[7] * f;
		ret[8] = d[8] * f;  ret[9] = d[9] * f;  ret[10] = d[10] * f;  ret[11] = d[11] * f;  
		ret[12] = d[12] * f;  ret[13] = d[13] * f;  ret[14] = d[14] * f;  ret[15] = d[15] * f;
		return ret;
	}
	Matrix4x4 operator / ( float f ) const
	{
		Matrix4x4 ret;
		ret[0] = d[0] / f;  ret[1] = d[1] / f;  ret[2] = d[2] / f;  ret[3] = d[3] / f;  
		ret[4] = d[4] / f;  ret[5] = d[5] / f;  ret[6] = d[6] / f;  ret[7] = d[7] / f;
		ret[8] = d[8] / f;  ret[9] = d[9] / f;  ret[10] = d[10] / f;  ret[11] = d[11] / f;  
		ret[12] = d[12] / f;  ret[13] = d[13] / f;  ret[14] = d[14] / f;  ret[15] = d[15] / f;
		return ret;
	}

	Matrix4x4 operator * ( const Matrix4x4 & rm ) const
	{
		Matrix4x4 ret;
		for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
		{
			ret(i,j) = m[i][0] * rm(0,j) +  m[i][1] * rm(1,j) +
					   m[i][2] * rm(2,j) +  m[i][3] * rm(3,j);
		}
		return ret;
	}

	Matrix4x4 Transpose()const
	{
		Matrix4x4 ret;

		ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
		ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
		ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
		ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;

		return ret;
	}

	Matrix4x4 Inverse()const
	{
		Matrix4x4 ret;

		ret.Identity();

		double detA = _11*_22*_33*_44 + _11*_23*_34*_42 + _11*_24*_32*_43
					+ _12*_21*_34*_43 + _12*_23*_31*_44 + _12*_24*_33*_41
					+ _13*_21*_32*_44 + _13*_22*_34*_41 + _13*_24*_31*_42
					+ _14*_21*_33*_42 + _14*_22*_31*_43 + _14*_23*_32*_41
					- _11*_22*_34*_43 - _11*_23*_32*_44 - _11*_24*_33*_42
					- _12*_21*_33*_44 - _12*_23*_34*_41 - _12*_24*_31*_43
					- _13*_21*_34*_42 - _13*_22*_31*_44 - _13*_24*_32*_41
					- _14*_21*_32*_43 - _14*_22*_33*_41 - _14*_23*_31*_42;

		if(detA == 0)
			return ret;
		double iDetA = 1 / detA;
		ret._11 = (_22*_33*_44 + _23*_34*_42 + _24*_32*_43 - _22*_34*_43 - _23*_32*_44 - _24*_33*_42) * iDetA;
		ret._12 = (_12*_34*_43 + _13*_32*_44 + _14*_33*_42 - _12*_33*_44 - _13*_34*_42 - _14*_32*_43) * iDetA;
		ret._13 = (_12*_23*_44 + _13*_24*_42 + _14*_22*_43 - _12*_24*_43 - _13*_22*_44 - _14*_23*_42) * iDetA;
		ret._14 = (_12*_24*_33 + _13*_22*_34 + _14*_23*_32 - _12*_23*_34 - _13*_24*_32 - _14*_22*_33) * iDetA;

		ret._21 = (_21*_34*_43 + _23*_31*_44 + _24*_33*_41 - _21*_33*_44 - _23*_34*_41 - _24*_31*_43) * iDetA;
		ret._22 = (_11*_33*_44 + _13*_34*_41 + _14*_31*_43 - _11*_34*_43 - _13*_31*_44 - _14*_33*_41) * iDetA;
		ret._23 = (_11*_24*_43 + _13*_21*_44 + _14*_23*_41 - _11*_23*_44 - _13*_24*_41 - _14*_21*_43) * iDetA;
		ret._24 = (_11*_23*_34 + _13*_24*_31 + _14*_21*_33 - _11*_24*_33 - _13*_21*_34 - _14*_23*_31) * iDetA;

		ret._31 = (_21*_32*_44 + _22*_34*_41 + _24*_31*_42 - _21*_34*_42 - _22*_31*_44 - _24*_32*_41) * iDetA;
		ret._32 = (_11*_34*_42 + _12*_31*_44 + _14*_32*_41 - _11*_32*_44 - _12*_34*_41 - _14*_31*_42) * iDetA;
		ret._33 = (_11*_22*_44 + _12*_24*_41 + _14*_21*_42 - _11*_24*_42 - _12*_21*_44 - _14*_22*_41) * iDetA;
		ret._34 = (_11*_24*_32 + _12*_21*_34 + _14*_22*_31 - _11*_22*_34 - _12*_24*_31 - _14*_21*_32) * iDetA;

		ret._41 = (_21*_33*_42 + _22*_31*_43 + _23*_32*_41 - _21*_32*_43 - _22*_33*_41 - _23*_31*_42) * iDetA;
		ret._42 = (_11*_32*_43 + _12*_33*_41 + _13*_31*_42 - _11*_33*_42 - _12*_31*_43 - _13*_32*_41) * iDetA;
		ret._43 = (_11*_23*_42 + _12*_21*_43 + _13*_22*_41 - _11*_22*_43 - _12*_23*_41 - _13*_21*_42) * iDetA;
		ret._44 = (_11*_22*_33 + _12*_23*_31 + _13*_21*_32 - _11*_23*_32 - _12*_21*_33 - _13*_22*_31) * iDetA;

		return ret;
	}
};

inline Matrix4x4 operator * ( float f, const Matrix4x4 &rm  )
{
	Matrix4x4 ret;
	ret[0] = rm[0] * f;  ret[1] = rm[1] * f;  ret[2] = rm[2] * f;  ret[3] = rm[3] * f;  
	ret[4] = rm[4] * f;  ret[5] = rm[5] * f;  ret[6] = rm[6] * f;  ret[7] = rm[7] * f;
	ret[8] = rm[8] * f;  ret[9] = rm[9] * f;  ret[10] = rm[10] * f;  ret[11] = rm[11] * f;  
	ret[12] = rm[12] * f;  ret[13] = rm[13] * f;  ret[14] = rm[14] * f;  ret[15] = rm[15] * f;
	return ret;
}
inline Vector3D operator * ( const Vector3D &v, const Matrix4x4 &rm )
{
	Vector3D ret; double w;
	for( int i=0; i<3; i++ )
		ret[i] =  v.x * rm(0,i) + v.y * rm(1,i) + v.z * rm(2,i) + 1.0 * rm(3,i);
	w = v.x * rm(0,3) + v.y * rm(1,3) + v.z * rm(2,3) + 1.0 * rm(3,3);
	double wInv = 1.0 / w;
	return ret * wInv;
}

inline Vector3D Mul3x3( const Vector3D &v, const Matrix4x4 &rm )
{
	Vector3D ret;
	for( int i=0; i<3; i++ )
		ret[i] =  v.x * rm(0,i) + v.y * rm(1,i) + v.z * rm(2,i);
	return ret;

}

inline Matrix4x4 Mat4x4Translate( double tx, double ty, double tz )
{
	Matrix4x4 ret;
	ret.Identity();
	ret._41 = tx; ret._42 = ty; ret. _43 = tz;
	return ret;
}

inline Matrix4x4 Mat4x4Scale( double sx, double sy, double sz )
{
	Matrix4x4 ret;
	ret.Identity();
	ret._11 = sx; ret._22 = sy; ret. _33 = sz;
	return ret;
}

inline Matrix4x4 Mat4x4RotateX( double theta )
{
	Matrix4x4 ret;
	ret.Identity();
	double cosTheta = cos( theta );
	double sinTheta = sin( theta );
	ret._22 = cosTheta; ret._23 = sinTheta; 
	ret._32 = -sinTheta; ret. _33 = cosTheta;
	return ret;
}

inline Matrix4x4 Mat4x4RotateY( double theta )
{
	Matrix4x4 ret;
	ret.Identity();
	double cosTheta = cos( theta );
	double sinTheta = sin( theta );
	ret._11 = cosTheta; ret._13 = -sinTheta; 
	ret._31 = sinTheta; ret. _33 = cosTheta;
	return ret;
}

inline Matrix4x4 Mat4x4RotateZ( double theta )
{
	Matrix4x4 ret;
	ret.Identity();
	double cosTheta = cos( theta );
	double sinTheta = sin( theta );
	ret._11 = cosTheta; ret._12 = sinTheta; 
	ret._21 = -sinTheta; ret. _22 = cosTheta;
	return ret;
}

#endif