#include "Algebra.h"

Vec3::Vec3()
{
	std::cout << "Default constructor" << std::endl;
	data = new float[3];
	for (int i = 0; i < 3; i++)
		data[i] = 0;
}

Vec3::Vec3(float x, float y, float z) 
{
	std::cout << "Arguement constructor" << std::endl;
	data = new float[3];
	data[0] = x;	data[1] = y;	data[2] = z;
}

Vec3::Vec3( Vec3& vec)
{
	std::cout << "Copy constructor" << std::endl;
	data = new float[3];
	for (int i = 0; i < 3; i++)
		data[i] = vec.data[i];
}

Vec3::Vec3(Vec3&& vec)
{
	std::cout << "Rvalue copy constructor" << std::endl;
	data = vec.data;
	vec.data = nullptr;
}

Vec3::~Vec3()
{
	if(data != nullptr)
	   delete[] data;
}

float Vec3::getX()
{
	return data[0];
}

float Vec3::getY()
{
	return data[1];
}

float Vec3::getZ()
{
	return data[2];
}

void Vec3::setX(float x)
{
	data[0] = x;
}

void Vec3::setY(float y)
{
	data[1] = y;
}

void Vec3::setZ(float z)
{
	data[2] = z;
}

float& Vec3::operator[](int n)
{
	if (n >= 0 && n < 3)
	{
		return data[n];
	}
	else
	{
		std::cerr << "Index out of bonds" << std::endl;
	}
}

float Vec3::magnitude()
{
	return sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
}

Vec3& Vec3::operator=(Vec3& vec)
{
	std::cout << "lvalue = " << std::endl;
	if (this == &vec) return *this;
	for (int i = 0; i < 3; i++)
		data[i] = vec.data[i];
	return *this;
}

Vec3& Vec3::operator=(Vec3&& vec)
{
	std::cout << "rvalue = " << std::endl;
	data = vec.data;
	vec.data = nullptr;
	return *this;
}

Vec3 Vec3::operator-()
{
	return Vec3(-data[0], -data[1], -data[2]);
}

Vec3 operator+(Vec3& a, Vec3& b)
{
	std::cout << "l,l" << std::endl;
	return Vec3(a.data[0] + b.data[0], a.data[1] + b.data[1], a.data[2] + b.data[2]);
}

Vec3 operator+(Vec3& a, Vec3&& b)
{
	std::cout << "l,r" << std::endl;
	for (int i = 0; i < 3; i++)
		b[i] = b[i] + a[i];
	return b;
} 

Vec3 operator+(Vec3&& a, Vec3& b)
{
	std::cout << "r,l" << std::endl;
	for (int i = 0; i < 3; i++)
		a[i] = a[i] + b[i];
	return a;
}


Vec3 operator+(Vec3&& a, Vec3&& b)
{
	
	std::cout << "r,r" << std::endl;
	for (int i = 0; i < 3; i++)
		a[i] = a[i] + b[i];
	return a;
}



Vec3 operator-(Vec3& a, Vec3& b)
{
	return Vec3(a.data[0] - b.data[0], a.data[1] - b.data[1], a.data[2] - b.data[2]);
}

Vec3 operator-(Vec3& a, Vec3&& b)
{
	for (int i = 0; i < 3; i++)
		b[i] = a[i] - b[i];
	return b;
}

Vec3 operator-(Vec3&& a, Vec3& b)
{
	for (int i = 0; i < 3; i++)
		a[i] = a[i] - b[i];
	return a;
}

Vec3 operator-(Vec3&& a, Vec3&& b)
{
	for (int i = 0; i < 3; i++)
		a[i] = a[i] - b[i];
	return a;
}

float operator*(Vec3& a, Vec3& b)
{
	return (a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2]);
}

float operator*(Vec3& a, Vec3&& b)
{
	return (a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2]);
}

float operator*(Vec3&& a, Vec3& b)
{
	return (a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2]);
}

float operator*(Vec3&& a, Vec3&& b)
{
	return (a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2]);
}

Vec3 operator%(Vec3& a, Vec3& b)
{
	return Vec3(a.data[1]*b.data[2] -a.data[2]*b.data[1], a.data[2]*b.data[0] - a.data[0]*b.data[2], a.data[0]*b.data[1]-a.data[1]*b.data[0]);
}

Vec3 operator%(Vec3& a, Vec3&& b)
{
	float x = a.data[1] * b.data[2] - a.data[2] * b.data[1];
	float y = a.data[2] * b.data[0] - a.data[0] * b.data[2];
	float z = a.data[0] * b.data[1] - a.data[1] * b.data[0];
	b[0] = x;	b[1] = y;	b[2] = z;
	return b;
}

Vec3 operator%(Vec3&& a, Vec3& b)
{
	float x = a.data[1] * b.data[2] - a.data[2] * b.data[1];
	float y = a.data[2] * b.data[0] - a.data[0] * b.data[2];
	float z = a.data[0] * b.data[1] - a.data[1] * b.data[0];
	a[0] = x;	a[1] = y;	a[2] = z;
	return a;
}

Vec3 operator%(Vec3&& a, Vec3&& b)
{
	float x = a.data[1] * b.data[2] - a.data[2] * b.data[1];
	float y = a.data[2] * b.data[0] - a.data[0] * b.data[2];
	float z = a.data[0] * b.data[1] - a.data[1] * b.data[0];
	b[0] = x;	b[1] = y;	b[2] = z;
	return b;
}


Vec3 operator*(float s, Vec3& vec)
{
	return Vec3(s * vec.data[0], s * vec.data[1], s * vec.data[2]);
}

Vec3 operator*(float f, Vec3&& v)
{
	for (int i = 0; i < 3; i++)
		v[i] = f * v[i];
	return v;
}

bool operator==(Vec3& a, Vec3& b)
{
	return (a.data[0] == b.data[0]) && (a.data[1] == b.data[1]) && (a.data[2] == b.data[2]);
}

bool operator==(Vec3& a, Vec3&& b)
{
	return (a.data[0] == b.data[0]) && (a.data[1] == b.data[1]) && (a.data[2] == b.data[2]);
}

bool operator==(Vec3&& a, Vec3& b)
{
	return (a.data[0] == b.data[0]) && (a.data[1] == b.data[1]) && (a.data[2] == b.data[2]);
}

bool operator==(Vec3&& a, Vec3&& b)
{
	return (a.data[0] == b.data[0]) && (a.data[1] == b.data[1]) && (a.data[2] == b.data[2]);
}

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	os << "[" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << "]";
	return os;
}


Vec4::Vec4()
{
	data = new float[4];
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

Vec4::Vec4(float r, float x, float y, float z)
{
	data = new float[4];
	data[0] = r; data[1] = x; data[2] = y; data[3] = z;
}

Vec4::Vec4(Vec4& vec)
{
	data = new float[4];
	for (int i = 0; i < 4; i++)
		data[i] = vec.data[i];
}

Vec4::Vec4(Vec4&& vec)
{
	data = vec.data;
	vec.data = nullptr;
}

Vec4::~Vec4()
{
	delete[] data;
}

float& Vec4::operator[](int n)
{
	if (n >= 0 && n < 4)
		return data[n];
	else
		std::cerr << " Vec4 index " << n << " is out of bounds " << std::endl;
}

float Vec4::magnitude()
{
	return sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + data[3]*data[3]);
}

Vec4& Vec4::operator=(const Vec4& vec)
{
	if (this == &vec) return *this;
	for (int i = 0; i < 4; i++)
		data[i] = vec.data[i];
	return *this;
}

Vec4& Vec4::operator=(Vec4&& vec)
{
	data = vec.data;
	vec.data = nullptr;
	return *this;
}

Vec4 Vec4::operator-()
{
	return Vec4(-data[0],-data[1],-data[2],-data[3]);
}

Vec4 operator+(Vec4& a, Vec4& b)
{
	return Vec4(a.data[0] + b.data[0], a.data[1] + b.data[1], a.data[2] + b.data[2], a.data[3] + b.data[3]);
}

Vec4 operator+(Vec4& a, Vec4&& b)
{
	for (int i = 0; i < 4; i++)
		b[i] = a[i] + b[i];
	return b;
}

Vec4 operator+(Vec4&& a, Vec4& b)
{
	for (int i = 0; i < 4; i++)
		a[i] = a[i] + b[i];
	return a;
}

Vec4 operator+(Vec4&& a, Vec4&& b)
{
	for (int i = 0; i < 4; i++)
		a[i] = a[i] + b[i];
	return a;
}

Vec4 operator-(Vec4& a, Vec4& b)
{
	return Vec4(a.data[0] - b.data[0], a.data[1] - b.data[1], a.data[2] - b.data[2], a.data[3] - b.data[3]);
}

Vec4 operator-(Vec4& a, Vec4&& b)
{
	for (int i = 0; i < 4; i++)
		b[i] = a[1] - b[i];
	return b;
}

Vec4 operator-(Vec4&& a, Vec4& b)
{
	for (int i = 0; i < 4; i++)
		a[i] = a[i] - b[i];
	return a;
}

Vec4 operator-(Vec4&& a, Vec4&& b)
{
	for (int i = 0; i < 4; i++)
		a[i] = a[i] - b[i];
	return a;
}

Vec4 operator*(float s, Vec4& vec)
{
	return Vec4(s*vec.data[0],s*vec.data[1],s*vec.data[2],s*vec.data[3]);
}

Vec4 operator*(float s, Vec4&& vec)
{
	for (int i = 0; i < 4; i++)
		vec[i] = s * vec[i];
	return vec;
}

std::ostream& operator<< (std::ostream& os, Vec4& vec)
{
	os << "[" << vec.data[0] << ", " << vec.data[1] << ", " << vec.data[2] << ", " << vec.data[3] << "]";
	return os;
}


Mat4::Mat4()
{
	data = new Vec4[4];
}

Mat4::Mat4(Mat4& m)
{
	data = new Vec4[4];
	for (int i = 0; i < 4; i++)
		data[i] = m[i];
}

Mat4::Mat4(Mat4&& m)
{
	data = m.data;
	m.data = nullptr;
}

Mat4::Mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	data = new Vec4[4];
	data[0][0] = m00;	data[0][1] = m01;	data[0][2] = m02;	data[0][3] = m03;
	data[1][0] = m10;	data[1][1] = m11;	data[1][2] = m12;	data[1][3] = m13;
	data[2][0] = m20;	data[2][1] = m21;	data[2][2] = m22;	data[2][3] = m23;
	data[3][0] = m30;	data[3][1] = m31;	data[3][2] = m32;	data[3][3] = m33;
}



Mat4::Mat4(Vec4& a, Vec4& b, Vec4& c, Vec4& d)
{
	data = new Vec4[4];
	data[0] = a;	data[1] = b;	data[2] = c;	data[3] = d;
}


Mat4& Mat4::operator=(Mat4& m)
{
	if (this == &m) return *this;

	for (int i = 0; i < 4; i++)
		data[i] = m.data[i];

}

Mat4& Mat4::operator=(Mat4&& m)
{
	data = m.data;
	m.data = nullptr;
	return *this;
}

Mat4::~Mat4()
{
	delete[] data;
}

Vec4& Mat4::operator[](int n)
{
	if (n >= 0 && n < 4)
		return data[n];
	else
		std::cerr << " Invalid coloumn index " << n << " of Mat4 " << std::endl;
}

void Mat4::identity()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				data[i][j] = 1;
			else
				data[i][j] = 0;
		}
	}
}

Vec4 operator*(Mat4& m, Vec4& v)
{
	Vec4 temp;
	for (int i = 0; i < 4; i++)
	{
		temp = temp + (v[i]*m[i]);
	}
	return temp;
}

Vec4 operator*(Mat4& m, Vec4&& v)
{
	Vec4 temp;
	for (int i = 0; i < 4; i++)
	{
		temp = temp + (v[i] * m[i]);
	}
	return temp;
}

Vec4 operator*(Mat4&& m, Vec4& v)
{
	m[0] = v[0] * m[0];
	for (int i = 1; i < 4; i++)
	{
		m[0] = m[0] + (v[i] * m[i]);
	}
	return m[0];
}

Vec4 operator*(Mat4&& m, Vec4&& v)
{
	m[0] = v[0] * m[0];
	for (int i = 1; i < 4; i++)
	{
		m[0] = m[0] + (v[i] * m[i]);
	}
	return m[0];
}

Mat4 operator*(Mat4& m, Mat4& n)
{
	Mat4 h;
	for (int i = 0; i < 4; i++)
		h[i] = m * n[i];
	return h;
}

Mat4 operator*(Mat4& m, Mat4&& n)
{
	
	for (int i = 0; i < 4; i++)
		n[i] = m * n[i];
	return n;
}

Mat4 operator*(Mat4&& m, Mat4& n)
{
	Mat4 h;  
	for (int i = 0; i < 4; i++)
		h[i] = m * n[i];
	return h;
}

Mat4 operator*(Mat4&& m, Mat4&& n)
{

	for (int i = 0; i < 4; i++)
		n[i] = m * n[i];
	return n;
}

Mat4 operator+(Mat4& m, Mat4& n)
{
	Mat4 h;
	for (int i = 0; i < 4; i++)
		h[i] = m[i] + n[i];
	return h;
}

Mat4 operator+(Mat4& m, Mat4&& n)
{
	for (int i = 0; i < 4; i++)
		n[i] = m[i] + n[i];
	return n;
}

Mat4 operator+(Mat4&& m, Mat4& n)
{
	for (int i = 0; i < 4; i++)
		m[i] = m[i] + n[i];
	return m;
}

Mat4 operator+(Mat4&& m, Mat4&& n)
{
	for (int i = 0; i < 4; i++)
		m[i] = m[i] + n[i];
	return m;
}

Mat4 operator-(Mat4& m, Mat4& n)
{
	Mat4 h;
	for (int i = 0; i < 4; i++)
		h[i] = m[i] - n[i];
	return h;
}
Mat4 operator-(Mat4& m, Mat4&& n)
{
	for (int i = 0; i < 4; i++)
		n[i] = m[i] - n[i];
	return n;
}

Mat4 operator-(Mat4&& m, Mat4& n)
{
	for (int i = 0; i < 4; i++)
		m[i] = m[i] - n[i];
	return m;
}

Mat4 operator-(Mat4&& m, Mat4&& n)
{
	for (int i = 0; i < 4; i++)
		m[i] = m[i] - n[i];
	return m;
}


std::ostream& operator<<(std::ostream& os, Mat4& m)
{																	
	  os << m[0][0] << " " <<  m[1][0] << " " << m[2][0] << " " << m[3][0] << "\n"
		 << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << "\n"
		 << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << "\n"
		 << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3];

	 return os;
}

double Quat::slerp_dot_threshHold = 0.9995f;

Quat::Quat()
{
}

Quat::Quat(Quat& q)
{
	r = q.r;
	v = q.v;
}

Quat::Quat(Quat&& q)
{
	this->v = q.v; // rvalue = 
	this->r = q.r;
}

Quat::Quat(float& r, float& x, float& y, float& z)
{
	this->r = r;
	v.setX(x);
	v.setY(y);
	v.setZ(z);
}

Quat::Quat(float r, Vec3& v)
{
	this->r = r;
	this->v = v;
}

Quat::Quat(float r, Vec3&& v)
{
	this->r = r;
	this->v = v;
}

float Quat::magnitude()
{
	return sqrt(r*r + v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

Quat Quat::conjugate()
{
	return Quat(r, -v);
}


Quat& Quat::operator=(Quat& q)
{
	this->r = r;
	this->v = v;
	return *this;
}

Quat& Quat::operator=(Quat&& q)
{
	this->v = q.v;
	this->r = q.r;
	return *this;
}


Mat4 Quat::toMat4()
{
	/*mat[0][0] = 1 - 2 * j$ * j$ - 2 * k$ * k$;		mat[1][0] = 2 * i$ * j$ - 2 * k$ * r$;		mat[2][0] = 2 * i$ * k$ + 2 * r$ * j$;
	mat[0][1] = 2 * i$ * j$ + 2 * r$ * k$;			mat[1][1] = 1 - 2 * i$ * i$ - 2 * k$ * k$;	mat[2][1] = 2 * j$ * k$ - 2 * r$ * i$;
	mat[0][2] = 2 * i$ * k$ - 2 * r$ * j$;			mat[1][2] = 2 * j$ * k$ + 2 * r$ * i$;		mat[2][2] = 1 - 2 * i$ * i$ - 2 * j$ * j$;
*/
	return Mat4( (1-2*v[1]*v[1]-2*v[2]*v[2]) , (2*v[0]*v[1]+2*r*v[2]), (2*v[0]*v[2]-2*r*v[1]), 0.0f,
				 (2*v[0]*v[1]-2*v[2]*r) , (1-2*v[0]*v[0]-2*v[2]*v[2]) , (2*v[1]*v[2]+2*r*v[0]), 0.0f,
				 (2*v[0]*v[2]+2*r*v[1]) , (2*v[1]*v[2]-2*r*v[0]) , (1-2*v[0]*v[0]-2*v[1]*v[1]), 0.0f,
				  0.0f,					   0.0f,					0.0f,						1.0f);
}

Quat::Quat(float& r, Vec3& v)
{
	this->r = r;
	this->v = v;
}

Quat operator+(Quat& a, Quat& b)
{
	return Quat(a.r + b.r, a.v + b.v);
}

Quat operator+(Quat& a, Quat&& b)
{
	b.r = a.r + b.r;
	b.v = a.v + b.v;
	return b;
}

Quat operator+(Quat&& a, Quat& b)
{
	a.r = a.r + b.r;
	a.v = a.v + b.v;
	return a;
}

Quat operator+(Quat&& a, Quat&& b)
{
	a.r = a.r + b.r;
	a.v = a.v + b.v;
	return a;
}

Quat operator-(Quat& a, Quat& b)
{
	return Quat(a.r-b.r, a.v-b.v);
}

Quat operator-(Quat& a, Quat&& b)
{
	b.r = a.r - b.r;
	b.v = a.v - b.v;
	return b;
}

Quat operator-(Quat&& a, Quat& b)
{
	a.r = a.r - b.r;
	a.v = a.v - b.v;
	return a;
}

Quat operator-(Quat&& a, Quat&& b)
{
	a.r = a.r - b.r;
	a.v = a.v - b.v;
	return a;
}

Quat operator*(Quat& a, Quat& b)
{
	return Quat(a.r*b.r - a.v*b.v, a.r*b.v + b.r*a.v + a.v%b.v);
}

Quat operator*(Quat& a, Quat&& b)
{
	float r = a.r * b.r - a.v * b.v;
	b.v = a.r * b.v + b.r * a.v + a.v % b.v;
	b.r = r;
	return b;
}

Quat operator*(Quat&& a, Quat& b)
{
	float r = a.r * b.r - a.v * b.v;
	a.v = a.r * b.v + b.r * a.v + a.v % b.v;
	a.r = r;
	return a;
}

Quat operator*(Quat&& a, Quat&& b)
{
	float r = a.r * b.r - a.v * b.v;
	a.v = a.r * b.v + b.r * a.v + a.v % b.v;
	a.r = r;
	return a;
}

Quat operator*(float s, Quat& q)
{
	return Quat(s*q.r, s*q.v);
}

Quat operator*(float s, Quat&& q)
{
	q.r = s * q.r;
	q.v = s * q.v;
	return q;
}

Quat slerp(Quat& q0, Quat& q1, float t)
{
	double cosA = (q0.r * q1.r + q0.v[0] * q1.v[0] + q0.v[1] * q1.v[1] + q0.v[2] * q1.v[2]) / (q0.magnitude() * q1.magnitude());
	if (cosA < 0.0)
	{
		q1 = q1.conjugate();
		q1.r = -q1.r;
		cosA = -cosA;
	}

	if (cosA > Quat::slerp_dot_threshHold)
	{
		Quat result = (1.0 - t) * q0 + t * q1;
		result = (1.0f/result.magnitude())*result;
		return result;
	}
	double alpha = acos(cosA);

	Quat q = (sin((1.0 - t) * alpha) / sin(alpha)) * q0 + (sin(t * alpha) / sin(alpha)) * q1;
	q = (1.0f/q.magnitude())*q;
	return q;
	
}
