#pragma once

#include "math/matrix/matrix.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{

class CoordinateSystems
{
private:
	static Matrix _get_rotation_matrix(const Vector &euler_angles_ZYX);

public:
	static Vector vec_body_to_inertial(const Vector &euler_angles_ZYX, const Vector &vec);

	static Vector vec_inertial_to_body(const Vector &euler_angles_ZYX, const Vector &vec);

	static Vector vec_eci_to_ecef(const Vector &vec_eci, uint64_t t_since_epoch);

	static Vector vec_ecef_to_eci(const Vector &vec_ecef, uint64_t t_since_epoch);

	static Vector vec_ecef_to_geodetic(const Vector &vec_ecef);
};

} // namespace math
} // namespace sat_sim
