#include <iostream>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor_model.hpp"
#include "magnetometer_sensor.hpp"
#include "math/astro_utils/astro_utils.hpp"
#include "math/coordinate_systems/coordinate_systems.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace core
{
namespace sensors
{

std::function<math::Vector(double, const math::Vector &)> MagnetometerSensor::_transfer_function;

void MagnetometerSensor::init()
{
	_transfer_function = magnetometer_sensor_model;
}

MagnetometerSensor::MagnetometerSensor()
    : Sensor()
{
	_val = math::Vector(0, 0, 0);
}

MagnetometerSensor::~MagnetometerSensor()
{
}

void MagnetometerSensor::update(uint64_t t_unix, double delta_t)
{
	math::Vector mag_vec = math::AstroUtils::get_mag_vec_eci(t_unix, _model->get_position_eci());
	math::Vector mag_vec_body = math::CoordinateSystems::vec_inertial_to_body(_model->get_rotation_euler_ZYX(), mag_vec);
	math::Vector mag_vec_mag_body = math::CoordinateSystems::vec_inertial_to_body(orientation, mag_vec);

	_val = _transfer_function(delta_t, mag_vec_mag_body);
}

math::Vector MagnetometerSensor::get_val()
{
	return _val;
}

} // namespace sensors
} // namespace core
} // namespace sat_sim
