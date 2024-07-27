#pragma once

#include "./outputs/ioutput.hpp"
#include "core/simulation/simulation_api/simulation_api.hpp"
#include "core/simulation_models/simulation_model.hpp"

#include <mutex>
#include <vector>

#include <stdint.h>

namespace sat_sim
{
namespace core
{

class Simulation
{
private:
	bool _stop_flag;
	bool _pause_flag;
	float _speed;
	uint64_t _time;
	std::mutex _time_mtx;

	SimulationModel *_sim_model;

	uint16_t _output_frequency;
	std::vector<outputs::IOutput *> _outputs;

	SimulationApi *_simulation_api;

	void _setup_api_callbacks();
	bool _request_connection_test_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);
	bool _request_pause_callback();
	bool _request_unpause_callback();
	bool _request_stop_callback();
	bool _request_eci_to_ecef_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);
	bool _request_ecef_to_eci_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);
	bool _request_ecef_to_geodetic_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);
	bool _request_get_sun_vec_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);
	bool _request_get_mag_vec_callback(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len);

	static void _simulation_api_task(Simulation *sim);

	static void _output_task(Simulation *sim);

public:
	Simulation(SimulationModel *sim_model, SimulationApi *simulation_api);

	~Simulation();

	void run();

	void pause();

	void unpause();

	void stop();

	void set_speed(float speed);

	void set_ouput_frequency(float freq);

	void add_output(outputs::IOutput *output);

	bool is_running();

	uint64_t get_time();
};

} // namespace core
} // namespace sat_sim
