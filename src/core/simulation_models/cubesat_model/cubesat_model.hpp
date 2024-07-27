#pragma once

#include <vector>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "core/simulation_models/simulation_model.hpp"
#include "math/physics_model/iphysics_model.hpp"

namespace sat_sim
{
namespace core
{

class CubesatModel : public SimulationModel
{
private:
	std::vector<sensors::Sensor *> _sensors;
	std::vector<sensors::SunSensor *> _sun_sensors;
	std::vector<sensors::MagnetometerSensor *> _magnetometer_sensors;

public:
	CubesatModel(math::physics_models::IPhysicsModel *physics_model);
	~CubesatModel();

	virtual void to_bytes(std::vector<uint8_t> &buffer) override;
};

} // namespace core
} // namespace sat_sim
