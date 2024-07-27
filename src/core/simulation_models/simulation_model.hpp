#pragma once

#include <stdint.h>
#include <vector>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "math/physics_model/iphysics_model.hpp"

namespace sat_sim
{
namespace core
{

class SimulationModel
{
protected:
	math::physics_models::IPhysicsModel *_physics_model;

	std::vector<sensors::Sensor *> _sensors;
	std::vector<sensors::SunSensor *> _sun_sensors;
	std::vector<sensors::MagnetometerSensor *> _magnetometer_sensors;

public:
	SimulationModel(math::physics_models::IPhysicsModel *physics_model);

	virtual ~SimulationModel();

	void update(uint64_t t, double delta_t);

	void add_sun_sensor(const sensors::SunSensor &sun_sens);

	void add_magnetometer_sensor(const sensors::MagnetometerSensor &sun_sens);

	virtual void to_bytes(std::vector<uint8_t> &buffer) = 0;
};

} // namespace core
} // namespace sat_sim
