#include "Quaternion.h"

Quaternion::Quaternion() : $r$(1), $i$(0), $j$(0), $k$(0)
{}

Quaternion::Quaternion(float w, float x, float y, float z) : $r$(w), $i$(x), $j$(y), $k$(z)
{}

Quaternion::Quaternion(float angle , Vector3f axis)
{
	float radAngle = (PI / 180.0f)*angle;
	float w = std::cos(radAngle / 2.0f);
	Vector3f vec = std::sin(radAngle / 2.0f) * axis.dirtection();
	$r$ = w;
	$i$ = vec.x();
	$j$ = vec.y();
	$k$ = vec.z();
}



Quaternion Quaternion::operator*(Quaternion quat)
{
	Vector3f victim(quat.$i$, quat.$j$, quat.$k$);
	Vector3f action($i$, $j$, $k$);
	float victimw = quat.$r$;
	float actionw = $r$;

	float resw = (victimw * actionw) - (action * victim);
	Vector3f resvec = (victimw * action) + (actionw * victim) + (action ^ victim);

	return Quaternion(resw, resvec.x(), resvec.y(), resvec.z());
}

Quaternion Quaternion::operator-()
{
	return Quaternion($r$, -$i$, -$j$, -$k$);
}

float Quaternion::length()
{
	return (float)(std::sqrt($r$*$r$ + $i$ * $i$ + $j$ * $j$ + $k$ * $k$));
}

void Quaternion::normalize()
{
	double length = std::sqrt($r$*$r$ + $i$ * $i$ + $j$ * $j$ + $k$ * $k$);
	$r$ = $r$ / length;
	$i$ = $i$ / length;
	$j$ = $j$ / length;
	$k$ = $k$ / length;
}



Matrix4f Quaternion::toMatrix4f()
{
	Matrix4f mat;
	mat[0][0] = 1 - (2 * $j$*$j$) - (2 * $k$*$k$);		mat[1][0] = 2 * $i$ * $j$ - 2 * $r$ * $k$;		mat[2][0] = 2 * $i$ * $k$ + 2 * $r$ * $j$;
	mat[0][1] = 2 * $i$*$j$ + 2 * $r$*$k$;				mat[1][1] = 1 - 2 * $i$*$i$ - 2 * $k$*$k$;		mat[2][1] = 2 * $j$ * $k$ - 2 * $r$ * $i$;
	mat[0][2] = 2 * $i$*$k$ - 2 * $r$*$j$;				mat[1][2] = 2 * $j$ * $k$ + 2 * $r$ * $i$;		mat[2][2] = 1 - 2*$i$*$i$ - 2*$j$ *$j$;

	return mat;
	
}

void Quaternion::operator=(Quaternion q)
{
	$r$ = q.$r$;
	$i$ = q.$i$;
	$j$ = q.$j$; 
	$k$ = q.$k$;
}

void Quaternion::values(float* arr, int start)
{
	arr[start + 0] = $r$;
	arr[start + 1] = $i$;
	arr[start + 2] = $j$;
	arr[start + 3] = $k$;
}

std::ostream & operator<<(std::ostream & os, Quaternion q)
{
	os << q.$r$ << " " << q.$i$ << " " << q.$j$ << " "<< q.$k$;
	return os;
}
