#include "core/actuators/magnetorquer_actuator/magnetorquer_actuator.hpp"
#include "math/astro_utils/astro_utils.hpp"

namespace sat_sim
{
namespace core
{
namespace actuators
{

void MagnetorquerActuator::update(uint64_t t, math::physics_models::IPhysicsModel &physics_model)
{
	math::Vector B = math::AstroUtils::get_mag_vec_eci(t, physics_model.get_position_eci());
	math::Vector m = {_in_x, _in_y, _in_z};

	physics_model.add_moments_body(m.cross(B));
}

math::Vector MagnetorquerActuator::get_moments()
{
	return {_out_x, _out_y, _out_z};
}

void MagnetorquerActuator::set_values(double x, double y, double z)
{
	_in_x = x;
	_in_y = y;
	_in_z = z;
}

} // namespace actuators
} // namespace core
} // namespace sat_sim
