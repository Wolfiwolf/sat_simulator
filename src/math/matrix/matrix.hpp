#pragma once

#include <stdint.h>
#include <string>

#include "math/quaternion/quaternion.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{

class Matrix
{
private:
	double **_rows = NULL;

public:
	uint8_t M;
	uint8_t N;

	Matrix();
	Matrix(uint8_t m, uint8_t n);
	Matrix(const Matrix &other);
	Matrix(Matrix &&other);
	~Matrix();

	double get_element(uint8_t m, uint8_t n);
	void set_element(uint8_t m, uint8_t n, double val);

	Vector to_vec();
	std::string to_string();

	Matrix add(const Matrix &mat);
	Matrix sub(const Matrix &mat);
	Matrix multiply(double scalar);
	Matrix multiply(const Matrix &mat);
	Vector multiply(const Vector &vec);
	Quaternion multiply(const Quaternion &q);
	Matrix transpose();
	double max();
	double min();

	Matrix operator=(const Matrix &other);
	double *operator[](uint8_t m);
	double *operator[](uint8_t m) const;
	Matrix operator+(const Matrix &mat);
	Matrix operator-(const Matrix &mat);
	Matrix operator*(const double &scalar);
	Matrix operator*(const Matrix &mat);
	Vector operator*(const Vector &vec);
	Quaternion operator*(const Quaternion &vec);
};

} // namespace math
} // namespace sat_sim
