#include <chrono>
#include <string.h>
#include <thread>
#include <unistd.h>

#include "./outputs/ioutput.hpp"
#include "core/simulation_models/simulation_model.hpp"
#include "math/astro_utils/astro_utils.hpp"
#include "math/coordinate_systems/coordinate_systems.hpp"
#include "simulation.hpp"

namespace sat_sim
{
namespace core
{

Simulation::Simulation(SimulationModel *sim_model, SimulationApi *simulation_api)
{
	_stop_flag = false;
	_pause_flag = false;
	_speed = 1.0f;
	_time = 0;

	_sim_model = sim_model;
	_simulation_api = simulation_api;

	_setup_api_callbacks();
}

Simulation::~Simulation()
{
	for (outputs::IOutput *o : _outputs)
		delete o;
}

void Simulation::run()
{
	std::thread simulation_api_thread(_simulation_api_task, this);
	std::thread output_thread(_output_task, this);
	double delta_t;

	simulation_api_thread.detach();
	output_thread.detach();

	while (!_stop_flag) {

		while (_pause_flag) {
		}

		delta_t = 100000;

		_time += (uint64_t)delta_t;
		delta_t /= 1000000.0;

		_sim_model->update(_time, delta_t);
	}
}

void Simulation::pause()
{
	_pause_flag = true;
}

void Simulation::unpause()
{
	_pause_flag = false;
}

void Simulation::stop()
{
	_stop_flag = true;
}

void Simulation::set_speed(float speed)
{
	_speed = speed;
}

void Simulation::set_ouput_frequency(float freq)
{
	_output_frequency = freq;
}

void Simulation::add_output(outputs::IOutput *output)
{
	_outputs.push_back(output);
}

bool Simulation::is_running()
{
	return !_pause_flag && !_stop_flag;
}

uint64_t Simulation::get_time()
{
	uint64_t t = _time;

	_time_mtx.lock();
	t = _time;
	_time_mtx.unlock();

	return t;
}

void Simulation::_output_task(Simulation *sim)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prev_t, current_t;
	uint64_t pause_t = (1 / (double)sim->_output_frequency) * 1000000ull;

	while (!sim->_stop_flag) {
		uint64_t duration;

		current_t = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(current_t - prev_t).count();

		if (duration < pause_t)
			continue;

		for (outputs::IOutput *o : sim->_outputs)
			o->output(sim->get_time());

		prev_t = std::chrono::high_resolution_clock::now();

		std::cout << "Juhu\n";
	}
}

void Simulation::_simulation_api_task(Simulation *sim)
{
	while (!sim->_stop_flag)
		sim->_simulation_api->wait_and_handle_request();
}

void Simulation::_setup_api_callbacks()
{
	Simulation *this_sim = this;

	_simulation_api->set_request_connection_test_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_connection_test_callback(data, data_len, buffer, buffer_len);
	    });

	_simulation_api->set_request_pause_callback([&this_sim]() { return this_sim->_request_pause_callback(); });

	_simulation_api->set_request_unpause_callback([&this_sim]() { return this_sim->_request_unpause_callback(); });

	_simulation_api->set_request_stop_callback([&this_sim]() { return this_sim->_request_stop_callback(); });

	_simulation_api->set_request_eci_to_ecef_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_eci_to_ecef_callback(data, data_len, buffer, buffer_len);
	    });

	_simulation_api->set_request_ecef_to_eci_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_ecef_to_eci_callback(data, data_len, buffer, buffer_len);
	    });

	_simulation_api->set_request_ecef_to_geodetic_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_ecef_to_geodetic_callback(data, data_len, buffer, buffer_len);
	    });

	_simulation_api->set_request_get_sun_vec_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_get_sun_vec_callback(data, data_len, buffer, buffer_len);
	    });

	_simulation_api->set_request_get_mag_vec_callback(
	    [&this_sim](uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len) {
		    return this_sim->_request_get_mag_vec_callback(data, data_len, buffer, buffer_len);
	    });
}

bool Simulation::_request_connection_test_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)
{
	buffer[0] = 1;
	buffer[1] = 2;
	buffer[2] = 3;
	*buffer_len = 3;

	return true;
}

bool Simulation::_request_pause_callback()
{
	return true;
}

bool Simulation::_request_unpause_callback()
{
	return true;
}

bool Simulation::_request_stop_callback()
{
	return true;
}

bool Simulation::_request_eci_to_ecef_callback(uint8_t *data, uint16_t data_len,
                                               uint8_t *buffer, uint16_t *buffer_len)
{
	using namespace sat_sim::math;

	uint64_t t;
	Vector eci, ecef;

	memcpy(&t, data, 8);

	memcpy(&eci[0], data + 8, 8 * 3);

	ecef = CoordinateSystems::vec_eci_to_ecef(eci, t);
	memcpy(buffer, &ecef[0], 8 * 3);

	*buffer_len = 8 * 3;

	return true;
}

bool Simulation::_request_ecef_to_eci_callback(uint8_t *data, uint16_t data_len,
                                               uint8_t *buffer, uint16_t *buffer_len)
{
	using namespace sat_sim::math;
	uint64_t t;
	Vector ecef, eci;

	memcpy(&t, data + 0, 8);

	memcpy(&ecef[0], data + 8, 8 * 3);

	eci = CoordinateSystems::vec_ecef_to_eci(ecef, t);
	memcpy(buffer, &eci[0], 8 * 3);

	*buffer_len = 8 * 3;

	return true;
}

bool Simulation::_request_ecef_to_geodetic_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)
{
	using namespace sat_sim::math;

	Vector ecef;
	Vector geodetic;

	memcpy(&ecef[0], data, 8 * 3);

	geodetic = CoordinateSystems::vec_ecef_to_geodetic(ecef);
	memcpy(buffer, &geodetic[0], 8 * 3);

	*buffer_len = 8 * 3;

	return true;
}

bool Simulation::_request_get_sun_vec_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)
{
	using namespace sat_sim::math;

	Vector eci, sun_vec;
	uint64_t t;
	uint8_t with_shadow;

	memcpy(&t, data, 8);

	memcpy(&eci[0], data + 8, 8 * 3);

	memcpy(&with_shadow, data + 8 * 4, 1);

	sun_vec = AstroUtils::get_sun_vec_eci(0, eci, with_shadow == 1);
	memcpy(buffer, &sun_vec[0], 8 * 3);

	*buffer_len = 8 * 3;

	return true;
}

bool Simulation::_request_get_mag_vec_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)
{
	using namespace sat_sim::math;

	Vector eci, mag_vec;
	uint64_t t;

	memcpy(&t, data, 8);

	memcpy(&eci[0], data + 8, 8 * 3);

	mag_vec = AstroUtils::get_mag_vec_eci(t, eci);
	memcpy(buffer, &mag_vec[0], 8 * 3);

	*buffer_len = 8 * 3;

	return true;
}
} // namespace core
} // namespace sat_sim
