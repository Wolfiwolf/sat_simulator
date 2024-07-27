#include "sun_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor_model.hpp"
#include "math/astro_utils/astro_utils.hpp"
#include "math/coordinate_systems/coordinate_systems.hpp"
#include "math/rotations/rotations.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace core
{
namespace sensors
{

std::function<double(double, const math::Vector &)> SunSensor::_transfer_function;

void SunSensor::init()
{
	_transfer_function = sun_sensor_model;
}

SunSensor::SunSensor()
    : Sensor(), _val(0)
{
}

SunSensor::~SunSensor()
{
}

void SunSensor::update(uint64_t t_unix, double delta_t)
{
	math::Vector sun_vec = math::AstroUtils::get_sun_vec_eci(t_unix, _model->get_position_eci(), true);
	math::Vector sun_vec_body = math::CoordinateSystems::vec_inertial_to_body(_model->get_rotation_euler_ZYX(), sun_vec);
	math::Vector sun_vec_body_euler = math::Rotations::vec_to_euler(sun_vec);
	math::Vector sun_vec_sens;

	if (is_on_sun[(uint32_t)sun_vec_body_euler[2]][(uint32_t)sun_vec_body_euler[1]]) {
		_val = _transfer_function(delta_t, {0, 0, 0});
		return;
	}

	sun_vec_sens = math::CoordinateSystems::vec_inertial_to_body(orientation, sun_vec);
	_val = _transfer_function(delta_t, sun_vec_sens);
}

double SunSensor::get_val()
{
	return _val;
}

} // namespace sensors
} // namespace core
} // namespace sat_sim
