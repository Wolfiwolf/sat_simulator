#include <cmath>

#include "math/coordinate_systems/coordinate_systems.hpp"
#include "math/quaternion/quaternion.hpp"
#include "rotations.hpp"

namespace sat_sim
{
namespace math
{

Quaternion Rotations::q_dot(const Quaternion &q, const Vector &w, double delta_t)
{
	Matrix omega(4, 4);
	const double wx = w[0];
	const double wy = w[1];
	const double wz = w[2];

	omega[0][0] = 0.0f;
	omega[0][1] = -wx;
	omega[0][2] = -wy;
	omega[0][3] = -wz;
	omega[1][0] = wx;
	omega[1][1] = 0.0f;
	omega[1][2] = wz;
	omega[1][3] = -wy;
	omega[2][0] = wy;
	omega[2][1] = -wz;
	omega[2][2] = 0.0f;
	omega[2][3] = wx;
	omega[3][0] = wz;
	omega[3][1] = wy;
	omega[3][2] = -wx;
	omega[3][3] = 0.0f;

	return omega * q * (0.5 * delta_t);
}

Vector Rotations::q_to_euler_ZYX(const Quaternion &q)
{
	Vector euler;
	float qs = q[0];
	float qx = q[1];
	float qy = q[2];
	float qz = q[3];
	float C23 = 2.0 * (qy * qz + qx * qs);
	float C33 = qs * qs - qx * qx - qy * qy + qz * qz;
	float C13 = 2.0 * (qx * qz - qy * qs);
	float C12 = 2.0 * (qx * qy + qz * qs);
	float C11 = qs * qs + qx * qx - qy * qy - qz * qz;

	if (C13 >= 1.0)
		C13 = 1.0;
	else if (C13 <= -1.0)
		C13 = -1.0;

	euler[0] = atan2(C23, C33);
	euler[1] = -asin(C13);
	euler[2] = atan2(C12, C11);

	return euler;
}

Matrix Rotations::euler_ZYX_to_q(const Vector &euler)
{
	Matrix q_h(4, 1), q(4, 1);
	double q_h_max;

	double phi = euler[0];
	double theta = euler[1];
	double psi = euler[2];

	double C11 = cos(theta) * cos(psi);
	double C12 = cos(theta) * sin(psi);
	double C13 = -sin(theta);

	double C21 = sin(phi) * sin(theta) * cos(psi) - cos(phi) * sin(psi);
	double C22 = cos(phi) * cos(psi) + sin(phi) * sin(theta) * sin(psi);
	double C23 = sin(phi) * cos(theta);

	double C31 = cos(phi) * sin(theta) * cos(psi) + sin(phi) * sin(psi);
	double C32 = cos(phi) * sin(theta) * sin(psi) - sin(phi) * cos(psi);
	double C33 = cos(phi) * cos(theta);

	q_h[0][0] = sqrt(fabs(0.25 * (1 + C11 + C22 + C33)));
	q_h[1][0] = sqrt(fabs(0.25 * (1 + C11 - C22 - C33)));
	q_h[2][0] = sqrt(fabs(0.25 * (1 - C11 + C22 - C33)));
	q_h[3][0] = sqrt(fabs(0.25 * (1 - C11 - C22 + C33)));

	q_h_max = q_h.max();

	if (q_h_max == q_h[0][0]) {
		q[0][0] = q_h_max;
		q[1][0] = (C23 - C32) / (4.0f * q_h_max);
		q[2][0] = (C31 - C13) / (4.0f * q_h_max);
		q[3][0] = (C12 - C21) / (4.0f * q_h_max);
	} else if (q_h_max == q_h[1][0]) {
		q[0][0] = (C23 - C32) / (4 * q_h_max);
		q[1][0] = q_h_max;
		q[2][0] = (C12 - C21) / (4 * q_h_max);
		q[3][0] = (C31 - C13) / (4 * q_h_max);
	} else if (q_h_max == q_h[2][0]) {
		q[0][0] = (C31 - C13) / (4 * q_h_max);
		q[1][0] = (C12 + C21) / (4 * q_h_max);
		q[2][0] = q_h_max;
		q[3][0] = (C23 + C32) / (4 * q_h_max);
	} else if (q_h_max == q_h[3][0]) {
		q[0][0] = (C12 - C21) / (4 * q_h_max);
		q[1][0] = (C31 + C13) / (4 * q_h_max);
		q[2][0] = (C23 + C32) / (4 * q_h_max);
		q[3][0] = q_h_max;
	}

	return q;
}

Vector Rotations::euler_of_vec(const Vector &euler_ZYX)
{
	return CoordinateSystems::vec_body_to_inertial(euler_ZYX, {1, 0, 0});
}

Vector Rotations::vec_to_euler(const Vector &vec)
{
	math::Vector x_proj_on_xy = vec.proj_onto_plane({0, 0, 1});

	double psi = x_proj_on_xy.angle_between({1, 0, 0});

	double theta = vec.angle_between(x_proj_on_xy);

	return {0, theta, psi};
}

} // namespace math
} // namespace sat_sim
