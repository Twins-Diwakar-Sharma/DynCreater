#ifndef H_A763BRA_H
#define H_A763BRA_H

#include <iostream>
#include <math.h>

class Vec3
{
private:
	float* data;
public:
	//float* data;
	// constructors
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(Vec3&);
	Vec3(Vec3&&);
	~Vec3();
	// getters setters
	float getX();	float getY();	float getZ();
	void setX(float);	void setY(float); void setZ(float);
	float& operator[](int);

	// maths
	float magnitude();
	Vec3& operator=(Vec3&);
	Vec3& operator=(Vec3&&);
	Vec3 operator-();
	friend Vec3 operator+(Vec3&, Vec3&);  friend Vec3 operator+(Vec3&, Vec3&&);  friend Vec3 operator+(Vec3&&, Vec3&);  friend Vec3 operator+(Vec3&&, Vec3&&);
	friend Vec3 operator-(Vec3&, Vec3&);  friend Vec3 operator-(Vec3&, Vec3&&);  friend Vec3 operator-(Vec3&&, Vec3&);  friend Vec3 operator-(Vec3&&, Vec3&&);
	friend float operator*(Vec3&, Vec3&); friend float operator*(Vec3&, Vec3&&); friend float operator*(Vec3&&, Vec3&); friend float operator*(Vec3&&, Vec3&&);
	friend Vec3 operator%(Vec3&, Vec3&);  friend Vec3 operator%(Vec3&, Vec3&&); friend Vec3 operator%(Vec3&&, Vec3&);  friend Vec3 operator%(Vec3&&, Vec3&&); 
	friend Vec3 operator*(float, Vec3&);
	friend Vec3 operator*(float, Vec3&&);


	// logical
	friend bool operator==(Vec3&, Vec3&); friend bool operator==(Vec3&, Vec3&&); friend bool operator==(Vec3&&, Vec3&); friend bool operator==(Vec3&&, Vec3&&);
	// output
	friend std::ostream & operator << (std::ostream& out, const Vec3& v);

};

class Vec4
{
private:
	float* data;
public:
	// constructors
	Vec4();
	Vec4(float r, float x,  float y, float z);
	Vec4(Vec4&);
	Vec4(Vec4&&);
	~Vec4();
	// getters and setters
	float& operator[](int);
	// mathematics
	float magnitude();
	Vec4& operator=(const Vec4&);
	Vec4& operator=(Vec4&&);
	Vec4 operator-();
	friend Vec4 operator+(Vec4&, Vec4&); friend Vec4 operator+(Vec4&, Vec4&&); friend Vec4 operator+(Vec4&&, Vec4&); friend Vec4 operator+(Vec4&&, Vec4&&);
	friend Vec4 operator-(Vec4&, Vec4&); friend Vec4 operator-(Vec4&, Vec4&&); friend Vec4 operator-(Vec4&&, Vec4&); friend Vec4 operator-(Vec4&&, Vec4&&);
	friend Vec4 operator*(float, Vec4&);
	friend Vec4 operator*(float, Vec4&&);
	// output
	friend std::ostream& operator<< (std::ostream&, Vec4&);
};

class Mat4
{
private:
	Vec4* data;
public:
	Mat4();
	Mat4(Mat4&);
	Mat4(Mat4&&);

	Mat4(float m00, float m01, float m02, float m03,
		 float m10, float m11, float m12, float m13,
		 float m20, float m21, float m22, float m23,
		 float m30, float m31, float m32, float m33);



	Mat4(Vec4& a, Vec4& b, Vec4& c, Vec4& d);

	~Mat4();
	// getters and setters
	Vec4& operator[](int);
	// mathematics
	void identity();
	friend Vec4 operator*(Mat4&, Vec4&); friend Vec4 operator*(Mat4&, Vec4&&); friend Vec4 operator*(Mat4&&, Vec4&); friend Vec4 operator*(Mat4&&, Vec4&&);
	friend Mat4 operator*(Mat4&, Mat4&); friend Mat4 operator*(Mat4&, Mat4&&); friend Mat4 operator*(Mat4&&, Mat4&); friend Mat4 operator*(Mat4&&, Mat4&&);
	friend Mat4 operator+(Mat4&, Mat4&); friend Mat4 operator+(Mat4&, Mat4&&); friend Mat4 operator+(Mat4&&, Mat4&); friend Mat4 operator+(Mat4&&, Mat4&&);
	friend Mat4 operator-(Mat4&, Mat4&); friend Mat4 operator-(Mat4&, Mat4&&); friend Mat4 operator-(Mat4&&, Mat4&); friend Mat4 operator-(Mat4&&, Mat4&&);
	Mat4& operator=(Mat4&); Mat4& operator=(Mat4&&);
	
	friend std::ostream& operator<< (std::ostream&, Mat4&);
};

class Quat
{
private:
	float r;
	Vec3 v;
	static double slerp_dot_threshHold;
public:
	Quat();
	Quat(Quat&);
	Quat(Quat&&);
	Quat(float& r, float& x, float& y, float& z);
	Quat(float& r, Vec3& v);
	Quat(float r, Vec3& v);
	Quat(float r, Vec3&& v);

	float magnitude();

	Quat conjugate();
	friend Quat operator+(Quat&, Quat&);  friend Quat operator+(Quat&, Quat&&);  friend Quat operator+(Quat&&, Quat&); friend Quat operator+(Quat&&, Quat&&);
	friend Quat operator-(Quat&, Quat&); friend Quat operator-(Quat&, Quat&&); friend Quat operator-(Quat&&, Quat&); friend Quat operator-(Quat&&, Quat&&);
	friend Quat operator*(Quat&, Quat&); friend Quat operator*(Quat&, Quat&&); friend Quat operator*(Quat&&, Quat&); friend Quat operator*(Quat&&, Quat&&);
	friend Quat operator*(float, Quat&);
	friend Quat operator*(float, Quat&&);

	Quat& operator=(Quat&); Quat& operator=(Quat&&);

	Mat4 toMat4();
	friend Quat slerp(Quat& q0, Quat& q1, float t);
};
#endif
