#include <fstream>
#include <iostream>
#include <string.h>
#include <string>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "simulation_model.hpp"

namespace sat_sim
{
namespace core
{

SimulationModel::SimulationModel(math::physics_models::IPhysicsModel *physics_model)
    : _physics_model(physics_model)
{
}

SimulationModel::~SimulationModel()
{
	for (sensors::Sensor *sens : _sensors)
		delete sens;
}

void SimulationModel::update(uint64_t t, double delta_t)
{
	_physics_model->update(t, delta_t);
}

void SimulationModel::add_sun_sensor(const sensors::SunSensor &sun_sens)
{
	sensors::SunSensor *sensor = new sensors::SunSensor();

	sensor->orientation = sun_sens.orientation;
	sensor->position = sun_sens.position;
	sensor->set_physics_model(_physics_model);

	sensor->is_on_sun = sun_sens.is_on_sun;

	_sun_sensors.push_back(sensor);
}

void SimulationModel::add_magnetometer_sensor(const sensors::MagnetometerSensor &mag_sens)
{
	sensors::MagnetometerSensor *sensor = new sensors::MagnetometerSensor();

	sensor->orientation = mag_sens.orientation;
	sensor->position = mag_sens.position;
	sensor->set_physics_model(_physics_model);

	_magnetometer_sensors.push_back(sensor);
}

} // namespace core
} // namespace sat_sim
