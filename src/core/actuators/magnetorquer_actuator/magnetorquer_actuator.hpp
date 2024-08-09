#pragma once

#include "core/actuators/iactuator.hpp"

namespace sat_sim
{
namespace core
{
namespace actuators
{

class MagnetorquerActuator : public IActuator
{
private:
	double _in_x, _in_y, _in_z;
	double _out_x, _out_y, _out_z;

public:
	void update(uint64_t t, math::physics_models::IPhysicsModel &physics_model);

	math::Vector get_moments();

	void set_values(double x, double y, double z);
};

} // namespace actuators
} // namespace core
} // namespace sat_sim
