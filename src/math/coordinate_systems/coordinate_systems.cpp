#include <cmath>

#include "coordinate_systems.hpp"

namespace sat_sim
{
namespace math
{

Vector CoordinateSystems::vec_eci_to_ecef(const Vector &vec_eci, uint64_t t_since_epoch)
{
	const double EARTH_ROT_RATE = 360.9856123035484; // deg/day
	double eciRotation, cosOmega, sinOmega;

	t_since_epoch -= 946728000000ull; // T0 is J2000

	eciRotation = EARTH_ROT_RATE * ((((t_since_epoch / 1000.0) / 60.0) / 60.0) / 24.0) + 280.46;
	cosOmega = cos(eciRotation * 0.0174532925);
	sinOmega = sin(eciRotation * 0.0174532925);

	return {cosOmega * vec_eci[0] - sinOmega * vec_eci[1],
	        -(cosOmega * vec_eci[1] + sinOmega * vec_eci[0]),
	        vec_eci[2]};
}

Vector CoordinateSystems::vec_ecef_to_eci(const Vector &vec_ecef, uint64_t t_since_epoch)
{
	const double EARTH_ROT_RATE = 360.9856123035484; // deg/day
	double eciRotation, cosOmega, sinOmega;

	t_since_epoch -= 946728000000ull;

	eciRotation = EARTH_ROT_RATE * ((((t_since_epoch / 1000.0) / 60.0) / 60.0) / 24.0) + 280.46;
	cosOmega = cos(eciRotation * 0.0174532925);
	sinOmega = sin(eciRotation * 0.0174532925);

	return {cosOmega * vec_ecef[0] - sinOmega * vec_ecef[1],
	        -(cosOmega * vec_ecef[1] + sinOmega * vec_ecef[0]),
	        vec_ecef[2]};
}

Vector CoordinateSystems::vec_ecef_to_geodetic(const Vector &vec_ecef)
{
	const double RAD_TO_DEG = 57.2957795;
	double lon, lat, alt;

	double a = 6378137.0;
	double f = 1.0 / 298.257223563;

	double b = a - f * a;
	double e = sqrt(pow(a, 2.0) - pow(b, 2.0)) / a;
	double clambda = atan2(vec_ecef[1], vec_ecef[0]);
	double p = sqrt(pow(vec_ecef[0], 2.0) + pow(vec_ecef[1], 2.0));
	double h_old = 0.0;

	double theta = atan2(vec_ecef[2], p * (1.0 - pow(e, 2.0)));
	double cs = cos(theta);
	double sn = sin(theta);
	double N = pow(a, 2.0) / sqrt(pow(a * cs, 2.0) + pow(b * sn, 2.0));
	double h = p / cs - N;

	while (fabs(h - h_old) > 1.0e-6) {
		h_old = h;
		theta = atan2(vec_ecef[2], p * (1.0 - pow(e, 2.0) * N / (N + h)));
		cs = cos(theta);
		sn = sin(theta);
		N = pow(a, 2.0) / sqrt(pow(a * cs, 2.0) + pow(b * sn, 2.0));
		h = p / cs - N;
	}

	lon = clambda * RAD_TO_DEG;
	lat = theta * RAD_TO_DEG;
	alt = h;

	return {lat, lon, alt};
}

Vector CoordinateSystems::vec_inertial_to_body(const Vector &euler_angles_ZYX, const Vector &vec)
{
	return _get_rotation_matrix(euler_angles_ZYX).transpose() * vec;
}

Vector CoordinateSystems::vec_body_to_inertial(const Vector &euler_angles_ZYX, const Vector &vec)
{
	return _get_rotation_matrix(euler_angles_ZYX) * vec;
}

Matrix CoordinateSystems::_get_rotation_matrix(const Vector &euler_angles_ZYX)
{
	Matrix Rx(3, 3), Ry(3, 3), Rz(3, 3);
	double phi = euler_angles_ZYX[0];
	double theta = euler_angles_ZYX[1];
	double psi = euler_angles_ZYX[2];

	Rx[0][0] = 1.0;
	Rx[0][1] = 0.0;
	Rx[0][2] = 0.0;
	Rx[1][0] = 0.0;
	Rx[1][1] = cos(phi);
	Rx[1][2] = -sin(phi);
	Rx[2][0] = 0.0;
	Rx[2][1] = sin(phi);
	Rx[2][2] = cos(phi);

	Ry[0][0] = cos(theta);
	Ry[0][1] = 0.0;
	Ry[0][2] = sin(theta);
	Ry[1][0] = 0.0;
	Ry[1][1] = 1;
	Ry[1][2] = 0.0;
	Ry[2][0] = -sin(theta);
	Ry[2][1] = 0.0;
	Ry[2][2] = cos(theta);

	Rz[0][0] = cos(psi);
	Rz[0][1] = -sin(psi);
	Rz[0][2] = 0.0;
	Rz[1][0] = sin(psi);
	Rz[1][1] = cos(psi);
	Rz[1][2] = 0.0;
	Rz[2][0] = 0.0;
	Rz[2][1] = 0.0;
	Rz[2][2] = 1.0;

	return Rz * Ry * Rx;
}

} // namespace math
} // namespace sat_sim
