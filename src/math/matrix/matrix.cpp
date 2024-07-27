#include <string.h>
#include <string>

#include "math/quaternion/quaternion.hpp"
#include "matrix.hpp"

namespace sat_sim
{
namespace math
{

Matrix::Matrix()
{
	M = 0;
	N = 0;
}

Matrix::Matrix(uint8_t m, uint8_t n)
{
	int i;

	M = m;
	N = n;

	_rows = new double *[m];
	for (i = 0; i < m; ++i) {
		int j;

		_rows[i] = new double[n];
		for (j = 0; j < n; ++j)
			_rows[i][j] = 0.0;
	}
}

Matrix::Matrix(const Matrix &other)
{
	int i;

	if (_rows != NULL) {
		for (i = 0; i < other.M; ++i)
			delete _rows[i];
		delete _rows;
	}

	M = other.M;
	N = other.N;

	_rows = new double *[other.M];
	for (i = 0; i < other.M; ++i) {
		_rows[i] = new double[other.N];
		memcpy(_rows[i], other[i], other.N * 8);
	}
}

Matrix::Matrix(Matrix &&other)
{
	M = other.M;
	N = other.N;
	_rows = other._rows;
	other._rows = nullptr;
}

Matrix::~Matrix()
{
	if (_rows != nullptr) {
		for (uint8_t i = 0; i < M; ++i)
			delete _rows[i];
		delete _rows;
		_rows = nullptr;
	}
}

double Matrix::get_element(uint8_t m, uint8_t n)
{
	return _rows[m][n];
}

void Matrix::set_element(uint8_t m, uint8_t n, double val)
{
	_rows[m][n] = val;
}

Vector Matrix::to_vec()
{
	return {_rows[0][0], _rows[1][0], _rows[0][0]};
}

std::string Matrix::to_string()
{
	std::string str("");
	int i, j;

	for (i = 0; i < M; ++i) {
		for (j = 0; j < N; ++j)
			str += std::to_string(_rows[i][j]) + ' ';
		if (i != M - 1)
			str += '\n';
	}

	return str;
}

// OPERATIONS

Matrix Matrix::add(const Matrix &mat)
{
	Matrix res = *this;
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			res[i][j] += mat[i][j];

	return res;
}

Matrix Matrix::sub(const Matrix &mat)
{
	Matrix res = *this;
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			res[i][j] -= mat[i][j];

	return res;
}

Matrix Matrix::multiply(double scalar)
{
	Matrix res = *this;
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			res[i][j] *= scalar;

	return res;
}

Matrix Matrix::multiply(const Matrix &mat)
{
	Matrix res(M, mat.N);
	double sum;
	int i, j, k;

	for (i = 0; i < res.M; ++i) {
		for (j = 0; j < res.N; ++j) {
			sum = 0;

			for (k = 0; k < N; ++k)
				sum += (*this)[i][k] * mat[k][j];

			res[i][j] = sum;
		}
	}

	return res;
}

Vector Matrix::multiply(const Vector &vec)
{
	Vector res;
	double sum;
	int i, j;

	for (i = 0; i < M; ++i) {
		sum = 0;

		for (j = 0; j < N; ++j)
			sum += (*this)[i][j] * vec[j];

		res[i] = sum;
	}

	return res;
}

Quaternion Matrix::multiply(const Quaternion &q)
{
	Quaternion res;
	double sum;
	int i, j;

	for (i = 0; i < M; ++i) {
		sum = 0;

		for (j = 0; j < N; ++j)
			sum += (*this)[i][j] * q[j];

		res[i] = sum;
	}

	return res;
}

Matrix Matrix::transpose()
{
	Matrix res(M, N);
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			res[i][j] = _rows[j][i];

	return res;
}

double Matrix::max()
{
	double max_val = _rows[0][0];
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			if (_rows[i][j] > max_val)
				max_val = _rows[i][j];

	return max_val;
}

double Matrix::min()
{
	double min_val = _rows[0][0];
	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j)
			if (_rows[i][j] < min_val)
				min_val = _rows[i][j];

	return min_val;
}

// OPERATORS

Matrix Matrix::operator=(const Matrix &other)
{
	int i;

	if (_rows != NULL) {
		for (i = 0; i < other.M; ++i)
			delete _rows[i];
		delete _rows;
	}

	M = other.M;
	N = other.N;

	_rows = new double *[other.M];
	for (i = 0; i < other.M; ++i) {
		_rows[i] = new double[other.N];
		memcpy(_rows[i], other[i], other.N * 8);
	}

	return *(this);
}

double *Matrix::operator[](uint8_t m)
{
	return _rows[m];
}

double *Matrix::operator[](uint8_t m) const
{
	return _rows[m];
}

Matrix Matrix::operator+(const Matrix &mat)
{
	return (*this).add(mat);
}

Matrix Matrix::operator-(const Matrix &mat)
{
	return (*this).sub(mat);
}

Matrix Matrix::operator*(const double &scalar)
{
	return (*this).multiply(scalar);
}

Matrix Matrix::operator*(const Matrix &mat)
{
	return (*this).multiply(mat);
}

Vector Matrix::operator*(const Vector &vec)
{
	return (*this).multiply(vec);
}

Quaternion Matrix::operator*(const Quaternion &q)
{
	return (*this).multiply(q);
}

} // namespace math
} // namespace sat_sim
