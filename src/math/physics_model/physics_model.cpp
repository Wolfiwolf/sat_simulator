#include <math.h>
#include <stdio.h>
#include <string.h>

#include "sgp4/SGP4.h"
#include "sgp4/Vector.h"

#include "math/coordinate_systems/coordinate_systems.hpp"
#include "math/quaternion/quaternion.hpp"
#include "math/rotations/rotations.hpp"
#include "physics_model.hpp"

namespace sat_sim
{
namespace math
{
namespace physics_models
{

PhysicsModel::PhysicsModel()
{
	_mass = 5.0;

	_position_eci[0] = 6371000 + 5000000;
	_position_eci[1] = 0;
	_position_eci[2] = 0;

	_velocity_eci[1] = 5718.0;

	_rotation_q_eci = {1.0, 0, 0, 0};
	_rotation_euler_ZYX_eci = {0, 0, 0};

	_inertial_matrix = math::Matrix(3, 3);
	_inertial_matrix[0][0] = 1.0;
	_inertial_matrix[1][1] = 1.0;
	_inertial_matrix[2][2] = 1.0;

	_inertial_matrix_inverse = _inertial_matrix;

	_sgp4_model = nullptr;

	_orbit_mode = OrbitMode::SGP4;
}

PhysicsModel::~PhysicsModel()
{
	if (_sgp4_model != nullptr)
		delete _sgp4_model;
}

void PhysicsModel::set_to_sgp4_orbit_mode(const std::string &tle1, const std::string &tle2)
{
	if (_sgp4_model != nullptr)
		delete _sgp4_model;

	_sgp4_model = new libsgp4::SGP4({tle1, tle2});
	_orbit_mode = OrbitMode::SGP4;
}

void PhysicsModel::update(uint64_t t, double delta_t)
{
	int i;

	_state_mtx[0].lock();
	_update_gravity_gradient(delta_t);
	_state_mtx[0].unlock();

	_state_mtx[1].lock();
	_update_velocity(t, delta_t);
	_state_mtx[1].unlock();

	_state_mtx[2].lock();
	_update_angular_velocity(delta_t);
	_state_mtx[2].unlock();

	_state_mtx[3].lock();
	_update_rotation(delta_t);
	_state_mtx[3].unlock();

	_state_mtx[4].lock();
	_update_position(t, delta_t);
	_state_mtx[4].unlock();

	_prev_forces_eci = _forces_eci;
	_prev_moments_body = _moments_body;
	for (i = 0; i < 3; ++i) {
		_forces_eci[i] = 0.0;
		_moments_body[i] = 0.0;
	}
}

void PhysicsModel::add_moments_body(const math::Vector &momentum)
{
	_moments_body = _moments_body + momentum;
}

math::Vector PhysicsModel::get_position_eci()
{
	math::Vector res;

	_state_mtx[4].lock();
	res = _position_eci;
	_state_mtx[4].unlock();

	return res;
}

math::Vector PhysicsModel::get_velocity_eci()
{
	math::Vector res;

	_state_mtx[1].lock();
	res = _velocity_eci;
	_state_mtx[1].unlock();
	return res;
}

math::Quaternion PhysicsModel::get_rotation_q_eci()
{
	math::Quaternion res;

	_state_mtx[3].lock();
	res = _rotation_q_eci;
	_state_mtx[3].unlock();

	return res;
}

math::Vector PhysicsModel::get_rotation_euler_ZYX()
{
	math::Vector res;

	_state_mtx[3].lock();
	res = _rotation_euler_ZYX_eci;
	_state_mtx[3].unlock();

	return res;
}

math::Vector PhysicsModel::get_angular_velocity_body()
{
	math::Vector res;

	_state_mtx[2].lock();
	res = _angular_velocity_body;
	_state_mtx[2].unlock();

	return res;
}

math::Vector PhysicsModel::get_forces_eci()
{
	math::Vector res;

	_state_mtx[3].lock();
	res = _prev_forces_eci;
	_state_mtx[3].unlock();

	return res;
}

math::Vector PhysicsModel::get_moments_eci()
{
	math::Vector res;

	_state_mtx[0].lock();
	res = _moments_body;
	_state_mtx[0].unlock();

	return res;
}

void PhysicsModel::_update_position(uint64_t t, double delta_t)
{
	if (_orbit_mode == OrbitMode::SGP4) {
		libsgp4::Vector vec = _sgp4_model->FindPosition((t / 1000000000.0)).Position();

		_position_eci[0] = vec.x * 1000.0;
		_position_eci[1] = vec.y * 1000.0;
		_position_eci[2] = vec.z * 1000.0;
	}
}

void PhysicsModel::_update_velocity(uint64_t t, double delta_t)
{
	if (_orbit_mode == OrbitMode::SGP4) {
		libsgp4::Vector vec = _sgp4_model->FindPosition(t / 1000000.0).Velocity();

		_velocity_eci[0] = vec.x * 1000.0;
		_velocity_eci[1] = vec.y * 1000.0;
		_velocity_eci[2] = vec.z * 1000.0;
	}
}

void PhysicsModel::_update_rotation(double delta_t)
{
	double mag;

	_rotation_q_eci = _rotation_q_eci + math::Rotations::q_dot(_rotation_q_eci, _angular_velocity_body,
	                                                           delta_t);

	mag = _rotation_q_eci.magnitude();
	_rotation_q_eci = _rotation_q_eci * (1 / mag);

	_rotation_euler_ZYX_eci = math::Rotations::q_to_euler_ZYX(_rotation_q_eci);
}

void PhysicsModel::_update_angular_velocity(double delta_t)
{
	_angular_velocity_body =
	    _angular_velocity_body +
	    ((_inertial_matrix_inverse * (_moments_body - (_angular_velocity_body.cross(_inertial_matrix * _angular_velocity_body)))) *
	     delta_t);
}

void PhysicsModel::_update_gravity_gradient(double delta_t)
{
	math::Vector rb = math::CoordinateSystems::vec_inertial_to_body(_rotation_euler_ZYX_eci, _position_eci);
	double rbx = rb[0];
	double rby = rb[1];
	double rbz = rb[2];

	double Ix = _inertial_matrix[0][0];
	double Iy = _inertial_matrix[1][1];
	double Iz = _inertial_matrix[2][2];

	double u = 1.0;
	double moment_x = ((3 * u) / pow(_position_eci.magnitude(), 5)) * rby * rbz * (Iz - Iy);
	double moment_y = ((3 * u) / pow(_position_eci.magnitude(), 5)) * rbz * rbx * (Ix - Iz);
	double moment_z = ((3 * u) / pow(_position_eci.magnitude(), 5)) * rbx * rby * (Iy - Ix);

	math::Vector moments;

	moments[0] = moment_x;
	moments[1] = moment_y;
	moments[2] = moment_z;

	// add_moments_body(moments);
}

} // namespace physics_models
} // namespace math
} // namespace sat_sim
