#pragma once

#include <string>

#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{

class Quaternion
{
private:
	double *_vals;

public:
	Quaternion();
	Quaternion(double w, double x, double y, double z);
	Quaternion(const Quaternion &other);
	Quaternion(Quaternion &&other);
	~Quaternion();

	std::string to_string() const;

	Quaternion add(const Quaternion &q) const;
	Quaternion sub(const Quaternion &q) const;
	Quaternion multiply(const Quaternion &q) const;
	Quaternion multiply(double scalar) const;
	double magnitude() const;

	void create_from_axis(const Vector &axis, double degrees_rad);
	void rot_around_axis(const Vector &axis, double degrees_rad);

	Quaternion operator=(const Quaternion &other);
	double &operator[](uint8_t n);
	const double &operator[](uint8_t n) const;
	Quaternion operator+(const Quaternion &q);
	Quaternion operator-(const Quaternion &q) const;
	Quaternion operator*(const Quaternion &q) const;
	Quaternion operator*(const double &scalar) const;
};

} // namespace math
} // namespace sat_sim
