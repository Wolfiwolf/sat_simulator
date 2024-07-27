#pragma once

#include <string>

namespace sat_sim
{
namespace math
{

class Vector
{
private:
	double *_vals;

public:
	Vector();
	Vector(double x, double y, double z);
	Vector(const Vector &other);
	Vector(Vector &&other);
	~Vector();

	std::string to_string() const;

	Vector add(const Vector &vec) const;
	Vector sub(const Vector &vec) const;
	Vector multiply(double scalar) const;
	double magnitude() const;
	Vector cross(const Vector &vec) const;
	double dot(const Vector &vec) const;
	double angle_between(const Vector &vec) const;
	Vector proj_onto_vec(const Vector &vec) const;
	Vector proj_onto_plane(const Vector &normal) const;

	Vector operator=(const Vector &other);
	double &operator[](uint8_t n);
	const double &operator[](uint8_t n) const;
	Vector operator+(const Vector &vec);
	Vector operator-(const Vector &vec) const;
	double operator*(const Vector &vec) const;
	Vector operator*(const double &scalar) const;
};

} // namespace math
} // namespace sat_sim
