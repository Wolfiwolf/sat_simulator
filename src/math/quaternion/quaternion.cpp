#include <cmath>
#include <cstring>
#include <iostream>

#include "quaternion.hpp"

namespace sat_sim
{
namespace math
{

Quaternion::Quaternion()
{
	_vals = new double[4];
}

Quaternion::Quaternion(double w, double x, double y, double z)
{
	_vals = new double[4];

	_vals[0] = w;
	_vals[1] = x;
	_vals[2] = y;
	_vals[3] = z;
}

Quaternion::Quaternion(const Quaternion &other)
{
	_vals = new double[4];

	memcpy(_vals, other._vals, 8 * 4);
}

Quaternion::Quaternion(Quaternion &&other)
{
	_vals = other._vals;
	other._vals = nullptr;
}

Quaternion::~Quaternion()
{
	delete[] _vals;
}

std::string Quaternion::to_string() const
{
	return std::string("(") + std::to_string((*this)[0]) + ", " + std::to_string((*this)[1]) + ", " + std::to_string((*this)[2]) +
	       ", " + std::to_string((*this)[3]) + ")";
}

Quaternion Quaternion::add(const Quaternion &q) const
{
	Quaternion res;

	res[0] = (*this)[0] + q[0];
	res[1] = (*this)[1] + q[1];
	res[2] = (*this)[2] + q[2];
	res[3] = (*this)[3] + q[3];

	return res;
}

Quaternion Quaternion::sub(const Quaternion &q) const
{
	Quaternion res;

	res[0] = (*this)[0] - q[0];
	res[1] = (*this)[1] - q[1];
	res[2] = (*this)[2] - q[2];
	res[3] = (*this)[3] - q[3];

	return res;
}

Quaternion Quaternion::multiply(const Quaternion &q) const
{
	Quaternion q_res;

	double w1 = (*this)[0];
	double x1 = (*this)[1];
	double y1 = (*this)[2];
	double z1 = (*this)[3];

	double w2 = q[0];
	double x2 = q[1];
	double y2 = q[2];
	double z2 = q[3];

	q_res[0] = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	q_res[1] = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	q_res[2] = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
	q_res[3] = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;

	return q;
}

Quaternion Quaternion::multiply(double scalar) const
{
	Quaternion res;

	res[0] = (*this)[0] * scalar;
	res[1] = (*this)[1] * scalar;
	res[2] = (*this)[2] * scalar;
	res[3] = (*this)[3] * scalar;

	return res;
}

double Quaternion::magnitude() const
{
	return sqrt((*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2] + (*this)[3] * (*this)[3]);
}

void Quaternion::create_from_axis(const Vector &axis, double degrees_rad)
{
	double mag;
	double factor = sin(degrees_rad / 2.0);

	(*this)[0] = cos(degrees_rad / 2.0);
	(*this)[1] = factor * axis[0];
	(*this)[2] = factor * axis[1];
	(*this)[3] = factor * axis[2];

	mag = (*this).magnitude();

	(*this) = (*this) * (1 / mag);
}

void Quaternion::rot_around_axis(const Vector &axis, double degrees_rad)
{
	Quaternion q;

	q.create_from_axis(axis, degrees_rad);

	(*this) = (*this) * q;
}

Quaternion Quaternion::operator=(const Quaternion &other)
{
	_vals[0] = other[0];
	_vals[1] = other[1];
	_vals[2] = other[2];
	_vals[3] = other[3];

	return (*this);
}

double &Quaternion::operator[](uint8_t n)
{
	return _vals[n];
}

const double &Quaternion::operator[](uint8_t n) const
{
	return _vals[n];
}

Quaternion Quaternion::operator+(const Quaternion &q)
{
	return (*this).add(q);
}

Quaternion Quaternion::operator-(const Quaternion &q) const
{
	return (*this).sub(q);
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
	return (*this).multiply(q);
}

Quaternion Quaternion::operator*(const double &scalar) const
{
	return (*this).multiply(scalar);
}

} // namespace math
} // namespace sat_sim
