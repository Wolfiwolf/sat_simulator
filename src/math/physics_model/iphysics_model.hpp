#pragma once

#include "math/matrix/matrix.hpp"
#include "math/quaternion/quaternion.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{
namespace physics_models
{

class IPhysicsModel
{
public:
	virtual void set_to_sgp4_orbit_mode(const std::string &tle1, const std::string &tle2) = 0;

	virtual void update(uint64_t t, double delta_t) = 0;

	virtual void add_moments_body(const math::Vector &momentum) = 0;

	virtual math::Vector get_position_eci() = 0;
	virtual math::Vector get_velocity_eci() = 0;
	virtual math::Quaternion get_rotation_q_eci() = 0;
	virtual math::Vector get_rotation_euler_ZYX() = 0;
	virtual math::Vector get_angular_velocity_body() = 0;
	virtual math::Vector get_forces_eci() = 0;
	virtual math::Vector get_moments_eci() = 0;
};

} // namespace physics_models
} // namespace math
} // namespace sat_sim
