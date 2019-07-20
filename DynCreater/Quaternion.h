#ifndef HYPERSPHERE_UNIT_RADIUS
#define HYPERSPHERE_UNIT_RADIUS

#include "Matrix4f.h"

class Quaternion
{
private :
	float $r$, $i$, $j$, $k$;
public:
	// constructors
	Quaternion();
	Quaternion(float, float, float, float);
	Quaternion(float angle, Vector3f axis);
	
	// calculations
	float length();
	void normalize();
	Quaternion operator*(Quaternion);
	Quaternion operator-();

	// conversion
	Matrix4f toMatrix4f();
	void operator=(Quaternion);

	// output
	friend std::ostream& operator << (std::ostream&, Quaternion);

	// getter setter
	void values(float* , int );
};

#endif