#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <math.h>

class vec3
{
public:
	///our vector data
    float v[3];
    //constructors
    vec3();
	vec3(float x, float y, float z = 0.0f);
	vec3(const float* u);
	vec3(const vec3&);
	///returns an element of the vector
	float& operator[](unsigned);
	///assignment operator
    vec3& operator=(const vec3&);
	
	//arithmetic assignments
	///adds the input vector to this one
	vec3& operator+=(const vec3&);
	///subtracts the input vector from this one
	vec3& operator-=(const vec3&);
	///scales this vector with the scalar
    vec3& operator*=(float);
	///scales this vector with the inverse of the scalar
	vec3& operator/=(float);
	
	//arithmetic operations
	///adds two vectors
	const vec3 operator+(const vec3&) const;
	///subtracts two vectors
	const vec3 operator-(const vec3&) const;
	///scales the vector with the scalar
	const vec3 operator*(float) const;
	///scales the vector with the inverse of the scalar
	const vec3 operator/(float) const;

	//boolean operators
	///returns true if the vectors are equal
	bool operator==(const vec3&) const;
	///returns true if the vectors differ in at least one component
	bool operator!=(const vec3&) const;

	///opposite vector
	vec3& operator-();
	///norm of the vector (length*length)
	float norm() const;
	///length of the vector
	float length() const;
	///normalizes the vector
	vec3& operator!();
	///norm of the vectors' difference (dist*dist)
	float dist2(const vec3&);
	///length of the vectors' difference
	float dist(const vec3&);
	///dot product
	float operator*(const vec3&) const;
	///cross product
	const vec3 operator^(const vec3&) const;

	///print the vector components
	void print();
};

///output stream for the vector class
std::ostream& operator<<(std::ostream&, const vec3&);

#endif
