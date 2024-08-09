#pragma once

#include "math/physics_model/iphysics_model.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace core
{
namespace actuators
{

class IActuator
{
public:
	virtual void update(uint64_t t, math::physics_models::IPhysicsModel &physics_model) = 0;

	virtual math::Vector get_moments() = 0;
};

} // namespace actuators
} // namespace core
} // namespace sat_sim
