#pragma once

#include "math/physics_model/iphysics_model.hpp"

namespace sat_sim
{
namespace core
{
namespace sensors
{

class Sensor
{
protected:
	math::physics_models::IPhysicsModel *_model;

public:
	math::Vector position;
	math::Vector orientation;

	inline Sensor()
	{
	}

	inline virtual ~Sensor()
	{
	}

	inline void set_physics_model(math::physics_models::IPhysicsModel *model)
	{
		_model = model;
	}

	virtual void update(uint64_t t_unix, double delta_t) = 0;
};

} // namespace sensors
} // namespace core
} // namespace sat_sim
