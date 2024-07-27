#pragma once

#include <mutex>

#include "sgp4/SGP4.h"

#include "./iphysics_model.hpp"
#include "./physics_model.hpp"
#include "math/matrix/matrix.hpp"
#include "math/quaternion/quaternion.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace math
{
namespace physics_models
{

class PhysicsModel : public IPhysicsModel
{
public:
	enum class OrbitMode {
		SGP4 = 0
	};

private:
	std::mutex _state_mtx[5];
	math::Vector _forces_eci;
	math::Vector _prev_forces_eci;
	math::Vector _moments_body;
	math::Vector _prev_moments_body;

	void _update_position(uint64_t t, double delta_t);
	void _update_velocity(uint64_t t, double delta_t);
	void _update_rotation(double delta_t);
	void _update_angular_velocity(double delta_t);
	void _update_gravity_gradient(double delta_t);

protected:
	double _mass;
	math::Vector _position_eci;
	math::Vector _velocity_eci;
	math::Quaternion _rotation_q_eci;
	math::Vector _rotation_euler_ZYX_eci;
	math::Vector _angular_velocity_body;

	math::Matrix _inertial_matrix;
	math::Matrix _inertial_matrix_inverse;

	libsgp4::SGP4 *_sgp4_model;

	OrbitMode _orbit_mode;

public:
	PhysicsModel();
	~PhysicsModel();

	void set_to_sgp4_orbit_mode(const std::string &tle1, const std::string &tle2);

	void update(uint64_t t, double delta_t);

	void add_moments_body(const math::Vector &momentum);

	math::Vector get_position_eci();
	math::Vector get_velocity_eci();
	math::Quaternion get_rotation_q_eci();
	math::Vector get_rotation_euler_ZYX();
	math::Vector get_angular_velocity_body();
	math::Vector get_forces_eci();
	math::Vector get_moments_eci();
};

} // namespace physics_models
} // namespace math
} // namespace sat_sim
