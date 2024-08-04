#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <signal.h>
#include <string>
#include <thread>

#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "core/simulation/outputs/data_stream_output/data_stream_output.hpp"
#include "core/simulation/outputs/file_output/file_output.hpp"
#include "core/simulation/simulation.hpp"
#include "core/simulation/simulation_api/data_stream_simulation_api/data_stream_simulation_api.hpp"
#include "core/simulation_models/cubesat_model/cubesat_model.hpp"
#include "data_streams/data_streams/idata_stream.hpp"
#include "data_streams/data_streams/tcp_data_stream/tcp_data_stream.hpp"
#include "data_streams/data_streams/web_socket_data_stream/web_socket_data_stream.hpp"
#include "debug/logger/logger.hpp"
#include "file_processing/config_file/config_file.hpp"
#include "file_processing/simulation_model_file/simulation_model_file.hpp"
#include "files/model_3d_file/model_3d_file.hpp"
#include "files/model_3d_file/obj_file/obj_file.hpp"
#include "math/matrix/matrix.hpp"
#include "math/physics_model/physics_model.hpp"
#include "storage/cache_manager/cache_manager.hpp"
#include "storage/cache_manager/cache_storages/file_cache_storage/file_cache_storage.hpp"
#include "storage/cache_manager/cache_storages/icache_storage.hpp"
#include "system/system_info/linux_system_info/linux_system_info.hpp"

static void signal_sigint_callback_handler(int signum);

static void stop_all_operations();

using namespace sat_sim;

static sat_sim::core::Simulation *_simulation;

int main()
{
	storage::CacheManager cache_manager;
	system::LinuxSystemInfo system_info;
	storage::FileCacheStorage cache_storage(&system_info);
	file_processing::ConfigFile config_file;
	file_processing::SimulationModelFile sim_model_file;
	data_stream::IDataStream *data_stream;
	files::models_3d::Model3dFile *model_file;
	math::physics_models::PhysicsModel physics_model;
	core::CubesatModel sat(&physics_model);
	core::DataStreamSimulationApi *ds_simulation_api;
	math::Matrix inertia_matrix(3, 3);
	
	sat_sim::debug::Logger::Log("### Sat Simulator ###", sat_sim::debug::LogLevel::SUCCESS);

	// Setting up signal handlers
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_sigint_callback_handler);

	// Get system information
	system_info.init();

	// Initialize cache manager
	cache_manager.init(&cache_storage);

	// Reading config file
	if (!config_file.load("conf.txt")) {
		sat_sim::debug::Logger::Log("Error in loading config file!", sat_sim::debug::LogLevel::DANGER);
		return -1;
	}

	// Init PhysicsModel
	inertia_matrix[0][0] = config_file.inertia_matrix[0];
	inertia_matrix[0][1] = config_file.inertia_matrix[1];
	inertia_matrix[0][2] = config_file.inertia_matrix[2];

	inertia_matrix[1][0] = config_file.inertia_matrix[3];
	inertia_matrix[1][1] = config_file.inertia_matrix[4];
	inertia_matrix[1][2] = config_file.inertia_matrix[5];

	inertia_matrix[2][0] = config_file.inertia_matrix[6];
	inertia_matrix[2][1] = config_file.inertia_matrix[7];
	inertia_matrix[2][2] = config_file.inertia_matrix[8];

	physics_model.init(config_file.mass, inertia_matrix, config_file.tle1, config_file.tle2);

	// Setting up selected data stream
	if (config_file.data_stream_type == "web_socket")
		data_stream = new sat_sim::data_stream::WebSocketDataStream(config_file.data_stream_port);
	else if (config_file.data_stream_type == "tcp_ip")
		data_stream = new sat_sim::data_stream::TCPDataStream(config_file.data_stream_port);
	else
		data_stream = new sat_sim::data_stream::WebSocketDataStream(config_file.data_stream_port);

	data_stream->start();

	// Initializing sensors
	sat_sim::core::sensors::SunSensor::init();
	sat_sim::core::sensors::MagnetometerSensor::init();

	// Loading simulation model file
	model_file = new sat_sim::files::models_3d::ObjFile(config_file.model_file_path);
	model_file->load();
	sim_model_file.load(model_file, &cache_manager);
	delete model_file;

	// Creating the cubesat model
	for (const sat_sim::core::sensors::MagnetometerSensor &sens : sim_model_file.magnetometer_sensors)
		sat.add_magnetometer_sensor(sens);
	for (const sat_sim::core::sensors::SunSensor &sens : sim_model_file.sun_sensors)
		sat.add_sun_sensor(sens);

	ds_simulation_api = new core::DataStreamSimulationApi(data_stream);
	_simulation = new sat_sim::core::Simulation(&sat, ds_simulation_api);

	// Adding outputs
	for (const std::string &en_output : config_file.enabled_ouputs) {
		if (en_output == "file")
			_simulation->add_output(new sat_sim::outputs::FileOutput(&sat));
		else if (en_output == "data_stream")
			_simulation->add_output(new sat_sim::outputs::DataStreamOutput(&sat, data_stream));
	}

	_simulation->set_speed(100.0);
	_simulation->set_ouput_frequency(20.0);

	sat_sim::debug::Logger::Log("--- Running Simulator ---", sat_sim::debug::LogLevel::SUCCESS);

	_simulation->run();

	data_stream->stop();

	while (data_stream->is_up()) {
	}

	delete _simulation;
	delete ds_simulation_api;
	delete data_stream;

	return 0;
}

static void signal_sigint_callback_handler(int signum)
{
	sat_sim::debug::Logger::Log("Stopping simulator...", sat_sim::debug::LogLevel::DANGER);
	_simulation->stop();
}
