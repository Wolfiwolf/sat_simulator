#pragma once

#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{

class AstroUtils
{
public:
	static const uint64_t EARTH_R;

	static Vector get_sun_vec_eci(uint64_t t_unix, const Vector &pos_eci, bool with_shadow);

	static Vector get_mag_vec_eci(uint64_t t_unix, const Vector &pos_eci);

	static Vector get_euler_angles_ZYX_from_telemetry(uint64_t t_unix, const Vector &eci_pos,
	                                                  const Vector &sun_vec_body, const Vector &mag_vec_body);
};

} // namespace math
} // namespace sat_sim
