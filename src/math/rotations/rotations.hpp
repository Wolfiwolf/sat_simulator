#pragma once

#include "../matrix/matrix.hpp"
#include "../vector/vector.hpp"
#include "math/quaternion/quaternion.hpp"

namespace sat_sim
{
namespace math
{

class Rotations
{
public:
	static Vector q_to_euler_ZYX(const Quaternion &q);

	static Matrix euler_ZYX_to_q(const Vector &euler);

	static Quaternion q_dot(const Quaternion &q, const Vector &w, double delta_t);

	static Vector euler_of_vec(const Vector &euler_ZYX);

	static Vector vec_to_euler(const Vector &vec);
};

} // namespace math
} // namespace sat_sim
