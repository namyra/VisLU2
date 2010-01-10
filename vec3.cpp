#include "vec3.h"

#include <iostream>

vec3::vec3()
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}

vec3::vec3(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

vec3::vec3(const float* u)
{
	v[0] = u[0];
	v[1] = u[1];
	v[2] = u[2];
}

//the copy constructor
vec3::vec3(const vec3& u)
{
	v[0] = u.v[0];
	v[1] = u.v[1];
	v[2] = u.v[2];
}

float& vec3::operator[](unsigned i)
{
	return v[i]; //return the i-th component, allow also assignment
}

vec3& vec3::operator=(const vec3& u)
{
	if (this != &u) //don't try to assign myself to me
	{
		v[0] = u.v[0];
		v[1] = u.v[1];
		v[2] = u.v[2];
	}
	return *this;
}
//adds the value of u to this instance
vec3& vec3::operator+=(const vec3& u)
{
	v[0] += u.v[0];
	v[1] += u.v[1];
	v[2] += u.v[2];
	return *this;
}

vec3& vec3::operator-=(const vec3& u)
{
	v[0] -= u.v[0];
	v[1] -= u.v[1];
	v[2] -= u.v[2];
	return *this;
}

vec3& vec3::operator*=(float t)
{
	v[0] *= t;
	v[1] *= t;
	v[2] *= t;
	return *this;
}

vec3& vec3::operator/=(float t)
{
	return *this *= (1/t); //do the same as by multiplication only with 1/t
}

//adds this instance's value to u and returns a new instance with the result
const vec3 vec3::operator+(const vec3& u) const
{
	return vec3(*this) += u; //make a copy of myself, same as vec3 result *this; use += to add v to the copy.
}

const vec3 vec3::operator-(const vec3& u) const
{
	return vec3(*this) -= u;
}

const vec3 vec3::operator*(float t) const
{
	return vec3(*this) *= t;
}

const vec3 vec3::operator/(float t) const
{
	return vec3(*this) *= (1/t); 
}
//the opposite vector
vec3& vec3::operator-()
{
	return (*this) *= -1.0f;
}
//norm, the dot product of the vector with himself
inline float vec3::norm() const
{
	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}
//euclidian distance
inline float vec3::length() const
{
	return sqrt(norm());
}
//normalizes the vector
vec3& vec3::operator!()
{
	return *this /= length();
}
//norm of the difference of u and this vector instance
float vec3::dist2(const vec3& u)
{
	return (*this - u).norm();
}
//length of the difference of u and this vector instance
float vec3::dist(const vec3& u)
{
	return (*this - u).length();
}
//dot product
inline float vec3::operator*(const vec3& u) const
{
	return v[0]*u.v[0] + v[1]*u.v[1] + v[2]*u.v[2]; //return the dot product
}
//cross product
const vec3 vec3::operator^(const vec3& u) const
{
	return vec3(v[1]*u.v[2] - v[2]*u.v[1], v[2]*u.v[0] - v[0]*u.v[2], v[0]*u.v[1] - v[1]*u.v[0]); //return the cross product
}

bool vec3::operator==(const vec3 &u) const
{
    return (v[0] == u.v[0])&&(v[1] == u.v[1])&&(v[2] == u.v[2]);
}

bool vec3::operator!=(const vec3 &u) const
{
    return (v[0] != u.v[0])||(v[1] != u.v[1])||(v[2] != u.v[2]);
}

void vec3::print()
{
	std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const vec3& u)
{
	os << u.v[0] << ", " << u.v[1] << ", " << u.v[2];
	return os;
}
