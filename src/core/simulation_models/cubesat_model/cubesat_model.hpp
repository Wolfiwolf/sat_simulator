#pragma once

#include <vector>

#include "core/sensors/sensor.hpp"
#include "core/simulation_models/simulation_model.hpp"
#include "math/physics_model/iphysics_model.hpp"

namespace sat_sim
{
namespace core
{

class CubesatModel : public SimulationModel
{
private:
	math::physics_models::IPhysicsModel *_physics_model;
	std::vector<sensors::Sensor *> _sensors;

public:
	CubesatModel(math::physics_models::IPhysicsModel *physics_model);

	~CubesatModel();

	virtual void update(uint64_t t, double delta_t) override;

	void add_sensor(sensors::Sensor *s);

	virtual void to_bytes(std::vector<uint8_t> &buffer) override;
};

} // namespace core
} // namespace sat_sim
