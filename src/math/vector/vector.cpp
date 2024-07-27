#include <cmath>
#include <iostream>

#include "vector.hpp"

namespace sat_sim
{
namespace math
{

Vector::Vector()
{
	_vals = new double[3];
}

Vector::Vector(double x, double y, double z)
{
	_vals = new double[3];
	_vals[0] = x;
	_vals[1] = y;
	_vals[2] = z;
}

Vector::Vector(const Vector &other)
{
	_vals = new double[3];

	_vals[0] = other[0];
	_vals[1] = other[1];
	_vals[2] = other[2];
}

Vector::Vector(Vector &&other)
{
	_vals = other._vals;
	other._vals = nullptr;
}

Vector::~Vector()
{
	delete[] _vals;
}

std::string Vector::to_string() const
{
	return std::string("(") + std::to_string((*this)[0]) + ", " + std::to_string((*this)[1]) + ", " + std::to_string((*this)[2]) + ")";
}

Vector Vector::add(const Vector &vec) const
{
	Vector res;

	res[0] = (*this)[0] + vec[0];
	res[1] = (*this)[1] + vec[1];
	res[2] = (*this)[2] + vec[2];

	return res;
}

Vector Vector::sub(const Vector &vec) const
{
	Vector res;

	res[0] = (*this)[0] - vec[0];
	res[1] = (*this)[1] - vec[1];
	res[2] = (*this)[2] - vec[2];

	return res;
}

Vector Vector::multiply(double scalar) const
{
	Vector res;

	res[0] = (*this)[0] * scalar;
	res[1] = (*this)[1] * scalar;
	res[2] = (*this)[2] * scalar;

	return res;
}

double Vector::magnitude() const
{
	return sqrt(this->dot(*this));
}

Vector Vector::cross(const Vector &vec) const
{
	Vector res;

	res[0] = ((*this)[1] * vec[2]) - ((*this)[2] * vec[1]);
	res[1] = ((*this)[2] * vec[0]) - ((*this)[0] * vec[2]);
	res[2] = ((*this)[0] * vec[1]) - ((*this)[1] * vec[0]);

	return res;
}

double Vector::dot(const Vector &vec) const
{
	return (*this)[0] * vec[0] + (*this)[1] * vec[1] + (*this)[2] * vec[2];
}

double Vector::angle_between(const Vector &vec) const
{
	double cos_of_angle = (*this).dot(vec) / (this->magnitude() * vec.magnitude());

	if (cos_of_angle > 1.0)
		cos_of_angle = 1.0;
	else if (cos_of_angle < -1.0)
		cos_of_angle = -1.0;

	return acos(cos_of_angle);
}

Vector Vector::proj_onto_vec(const Vector &vec) const
{
	return vec * ((*this).dot(vec) * (vec.magnitude()));
}

Vector Vector::proj_onto_plane(const Vector &normal) const
{
	return (*this) - (*this).proj_onto_vec(normal);
}

Vector Vector::operator=(const Vector &other)
{
	_vals[0] = other[0];
	_vals[1] = other[1];
	_vals[2] = other[2];

	return (*this);
}

double &Vector::operator[](uint8_t n)
{
	return _vals[n];
}

const double &Vector::operator[](uint8_t n) const
{
	return _vals[n];
}

Vector Vector::operator+(const Vector &vec)
{
	return (*this).add(vec);
}

Vector Vector::operator-(const Vector &vec) const
{
	return (*this).sub(vec);
}

double Vector::operator*(const Vector &vec) const
{
	return (*this).dot(vec);
}

Vector Vector::operator*(const double &scalar) const
{
	return (*this).multiply(scalar);
}

} // namespace math
} // namespace sat_sim
