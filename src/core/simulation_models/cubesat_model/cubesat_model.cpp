#include <regex>
#include <string.h>
#include <unordered_map>

#include "core/simulation_models/simulation_model.hpp"
#include "cubesat_model.hpp"
#include "math/quaternion/quaternion.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace core
{

CubesatModel::CubesatModel(math::physics_models::IPhysicsModel *physics_model)
    : SimulationModel(physics_model)
{
	_physics_model->set_to_sgp4_orbit_mode("1 25544U 98067A   08264.51782528 -.00002182  00000-0 -11606-4 0  2927",
	                                       "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537");
}

CubesatModel::~CubesatModel()
{
}

void CubesatModel::to_bytes(std::vector<uint8_t> &buffer)
{
	math::Vector pos = _physics_model->get_position_eci();
	math::Vector vel = _physics_model->get_velocity_eci();
	math::Quaternion rot_q = _physics_model->get_rotation_q_eci();
	math::Vector rot_euler = _physics_model->get_rotation_euler_ZYX();
	math::Vector angular_vel = _physics_model->get_angular_velocity_body();
	math::Vector forces = _physics_model->get_forces_eci();
	math::Vector moments = _physics_model->get_moments_eci();
	int i, offset;

	offset = 0;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &pos[0], 8);
	memcpy(&buffer[offset + 8], &pos[1], 8);
	memcpy(&buffer[offset + 16], &pos[2], 8);
	offset += 24;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &vel[0], 8);
	memcpy(&buffer[offset + 8], &vel[1], 8);
	memcpy(&buffer[offset + 16], &vel[2], 8);
	offset += 24;

	for (i = 0; i < 32; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &rot_q[0], 8);
	memcpy(&buffer[offset + 8], &rot_q[1], 8);
	memcpy(&buffer[offset + 16], &rot_q[2], 8);
	memcpy(&buffer[offset + 24], &rot_q[3], 8);
	offset += 32;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &rot_euler[0], 8);
	memcpy(&buffer[offset + 8], &rot_euler[1], 8);
	memcpy(&buffer[offset + 16], &rot_euler[2], 8);
	offset += 24;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &angular_vel[0], 8);
	memcpy(&buffer[offset + 8], &angular_vel[1], 8);
	memcpy(&buffer[offset + 16], &angular_vel[2], 8);
	offset += 24;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &forces[0], 8);
	memcpy(&buffer[offset + 8], &forces[1], 8);
	memcpy(&buffer[offset + 16], &forces[2], 8);
	offset += 24;

	for (i = 0; i < 24; ++i)
		buffer.push_back(0);
	memcpy(&buffer[offset + 0], &moments[0], 8);
	memcpy(&buffer[offset + 8], &moments[1], 8);
	memcpy(&buffer[offset + 16], &moments[2], 8);
	offset += 24;
}

} // namespace core
} // namespace sat_sim
