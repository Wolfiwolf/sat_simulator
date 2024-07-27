#include <filesystem>
#include <fstream>
#include <ios>
#include <math.h>
#include <regex>
#include <string>
#include <unordered_map>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "debug/logger/logger.hpp"
#include "debug/progress_bar/progress_bar.hpp"
#include "files/model_3d_file/model_3d_file.hpp"
#include "math/coordinate_systems/coordinate_systems.hpp"
#include "simulation_model_file.hpp"
#include "storage/cache_manager/icache_manager.hpp"

namespace sat_sim
{
namespace file_processing
{

bool SimulationModelFile::load(files::models_3d::Model3dFile *model_file, storage::ICacheManager *cache_manager)
{
	uint32_t signature = _get_file_signature(model_file->get_file_path());

	if (_was_file_processed(signature, cache_manager)) {
		_load_from_cache(signature, cache_manager);
	} else {
		_process_file(model_file);
		_mark_file_as_processed(model_file->get_file_path(), cache_manager);
	}

	return true;
}

bool SimulationModelFile::_process_file(files::models_3d::Model3dFile *model_file)
{
	// Proccess sensors
	std::regex sun_sens_reg("^SunSensor(\\d+)$");
	std::regex sun_sens_axis_reg("^SunSensor([X|Y|Z])(\\d+)$");
	std::regex mag_sens_reg("^MagnetometerSensor(\\d+)$");
	std::regex mag_sens_axis_reg("^MagnetometerSensor([X|Y|Z])(\\d+)$");

	std::unordered_map<uint8_t, math::Vector> sun_sensors;
	std::unordered_map<uint8_t, math::Vector[3]> sun_sensors_axis;

	std::unordered_map<uint8_t, math::Vector> mag_sensors;
	std::unordered_map<uint8_t, math::Vector[3]> mag_sensors_axis;

	for (const sat_sim::files::models_3d::ModelObject &obj : model_file->get_objects()) {
		std::smatch match;

		if (std::regex_match(obj.name, match, sun_sens_reg)) {
			math::Vector pos;
			uint32_t sens_index = std::stoi(match[1]);
			double x_average = 0.0, y_average = 0.0, z_average = 0.0;
			uint8_t num_of_verticies = 0;

			// Get sun sensor position
			for (const std::vector<sat_sim::files::models_3d::ModelVertex> &polygon : obj.polygons) {
				for (const sat_sim::files::models_3d::ModelVertex &vert : polygon) {
					x_average += vert.x;
					y_average += vert.y;
					z_average += vert.z;
					++num_of_verticies;
				}
			}

			pos[0] = x_average / num_of_verticies;
			pos[1] = y_average / num_of_verticies;
			pos[2] = z_average / num_of_verticies;

			sun_sensors[sens_index] = pos;

		} else if (std::regex_match(obj.name, match, sun_sens_axis_reg)) {
			uint32_t sens_index = std::stoi(match[2]);
			std::string axis = match[1];
			double x_average = 0.0, y_average = 0.0, z_average = 0.0;
			uint8_t num_of_verticies = 0;
			math::Vector pos;

			// Get sun sensor axis
			for (const std::vector<sat_sim::files::models_3d::ModelVertex> &polygon : obj.polygons) {
				for (const sat_sim::files::models_3d::ModelVertex &vert : polygon) {
					x_average += vert.x;
					y_average += vert.y;
					z_average += vert.z;
					++num_of_verticies;
				}
			}

			pos[0] = x_average / num_of_verticies;
			pos[1] = y_average / num_of_verticies;
			pos[2] = z_average / num_of_verticies;

			if (axis == "X")
				sun_sensors_axis[sens_index][0] = pos;
			else if (axis == "Y")
				sun_sensors_axis[sens_index][1] = pos;
			else if (axis == "Z")
				sun_sensors_axis[sens_index][2] = pos;
		} else if (std::regex_match(obj.name, match, mag_sens_reg)) {
			uint32_t sens_index = std::stoi(match[1]);
			double x_average = 0.0, y_average = 0.0, z_average = 0.0;
			uint8_t num_of_verticies = 0;
			math::Vector pos;

			// Get magnetometer position
			for (const std::vector<sat_sim::files::models_3d::ModelVertex> &polygon : obj.polygons) {
				for (const sat_sim::files::models_3d::ModelVertex &vert : polygon) {
					x_average += vert.x;
					y_average += vert.y;
					z_average += vert.z;
					++num_of_verticies;
				}
			}

			pos[0] = x_average / num_of_verticies;
			pos[1] = y_average / num_of_verticies;
			pos[2] = z_average / num_of_verticies;

			mag_sensors[sens_index] = pos;
		} else if (std::regex_match(obj.name, match, mag_sens_axis_reg)) {
			uint32_t sens_index = std::stoi(match[2]);
			std::string axis = match[1];
			double x_average = 0.0, y_average = 0.0, z_average = 0.0;
			uint8_t num_of_verticies = 0;
			math::Vector pos;

			// Get magnetometer axis
			for (const std::vector<sat_sim::files::models_3d::ModelVertex> &polygon : obj.polygons) {
				for (const sat_sim::files::models_3d::ModelVertex &vert : polygon) {
					x_average += vert.x;
					y_average += vert.y;
					z_average += vert.z;
					++num_of_verticies;
				}
			}

			pos[0] = x_average / num_of_verticies;
			pos[1] = y_average / num_of_verticies;
			pos[2] = z_average / num_of_verticies;

			if (axis == "X")
				mag_sensors_axis[sens_index][0] = pos;
			else if (axis == "Y")
				mag_sensors_axis[sens_index][1] = pos;
			else if (axis == "Z")
				mag_sensors_axis[sens_index][2] = pos;
		}
	}

	// Get sun sensor orientation
	for (auto &sun_sens_entry : sun_sensors) {
		math::Vector sun_sens_pos, axis_x, axis_y, axis_z, x_proj_on_xy, proj_normal;
		sat_sim::core::sensors::SunSensor sun_sensor;
		double psi, theta, phi;
		int sun_sens_id;

		sun_sens_id = sun_sens_entry.first;
		sun_sens_pos = sun_sens_entry.second;

		if (sun_sensors_axis.find(sun_sens_id) == sun_sensors_axis.end()) {
			debug::Logger::Log("Sun sensor " + std::to_string(sun_sens_id) + " doesn't have a axis!", debug::LogLevel::DANGER);
			return false;
		}

		axis_x = sun_sensors_axis[sun_sens_id][0] - sun_sens_pos;
		axis_y = sun_sensors_axis[sun_sens_id][1] - sun_sens_pos;
		axis_z = sun_sensors_axis[sun_sens_id][2] - sun_sens_pos;

		sun_sensor.position = sun_sens_pos;

		this->sun_sensors.push_back(sun_sensor);

		// Get psi angle
		x_proj_on_xy = axis_x.proj_onto_plane({0, 0, 1});
		psi = x_proj_on_xy.angle_between({1, 0, 0});

		// Get theta angle
		theta = axis_x.angle_between(x_proj_on_xy);

		// Get phi angle
		proj_normal = math::CoordinateSystems::vec_body_to_inertial({0, 0, psi}, {0, 1, 0});
		phi = axis_z.angle_between(axis_z.proj_onto_plane(proj_normal));

		sun_sensor.orientation = {phi, theta, psi};
	}

	// Get magnetometer orientation
	for (auto &mag_sens_entry : mag_sensors) {
		math::Vector mag_sens_pos, axis_x, axis_y, axis_z, x_proj_on_xy, proj_normal;
		double psi, theta, phi;
		int mag_sens_id;

		mag_sens_id = mag_sens_entry.first;
		mag_sens_pos = mag_sens_entry.second;

		if (mag_sensors_axis.find(mag_sens_id) == mag_sensors_axis.end()) {
			debug::Logger::Log("Magnetometer sensor " + std::to_string(mag_sens_id) + " doesn't have a axis!",
			                   debug::LogLevel::DANGER);
			return false;
		}


		axis_x = mag_sensors_axis[mag_sens_id][0] - mag_sens_pos;
		axis_y = mag_sensors_axis[mag_sens_id][1] - mag_sens_pos;
		axis_z = mag_sensors_axis[mag_sens_id][2] - mag_sens_pos;

		sat_sim::core::sensors::MagnetometerSensor mag_sensor;
		this->magnetometer_sensors.push_back(mag_sensor);
		this->magnetometer_sensors[this->magnetometer_sensors.size() - 1].position = mag_sens_pos;

		// Get psi angle
		x_proj_on_xy = axis_x.proj_onto_plane({0, 0, 1});
		psi = x_proj_on_xy.angle_between({1, 0, 0});

		// Get theta angle
		theta = axis_x.angle_between(x_proj_on_xy);

		// Get phi angle
		proj_normal = math::CoordinateSystems::vec_body_to_inertial({0, 0, psi}, {0, 1, 0});
		phi = axis_z.angle_between(axis_z.proj_onto_plane(proj_normal));

		mag_sensor.orientation = {phi, theta, psi};
	}

	debug::Logger::Log("Model loaded!", debug::LogLevel::NORMAL);
	debug::Logger::Log("Proccessing sun sensors...", debug::LogLevel::NORMAL);

	_process_sun_sensors(model_file);

	debug::Logger::Log("Sun sensors proccessed!", debug::LogLevel::NORMAL);

	return true;
}

void SimulationModelFile::_process_sun_sensors(sat_sim::files::models_3d::Model3dFile *model_file)
{
	debug::ProgressBar prog_bar(360, 50);
	std::regex sun_sens_reg("^SunSensor([X|Y|Z])*(\\d+)$");
	double psi, theta;

	for (psi = -180; psi < 180; psi += 1.0) {
		prog_bar.update(psi + 180);

		for (sat_sim::core::sensors::SunSensor &sun_sens : this->sun_sensors)
			sun_sens.is_on_sun.push_back({});

		for (theta = -90; theta < 90; theta += 1.0) {
			for (sat_sim::core::sensors::SunSensor &sun_sens : this->sun_sensors) {
				math::Vector sun_vec = math::CoordinateSystems::vec_body_to_inertial({0,
				                                                                      theta * 0.0174532925,
				                                                                      psi * 0.0174532925},
				                                                                     {1, 0, 0});

				bool does_model_cover_sens = false;

				for (const files::models_3d::ModelObject &obj : model_file->get_objects()) {
					// Is object sun sensor
					if (std::regex_match(obj.name, sun_sens_reg))
						continue;

					for (const std::vector<files::models_3d::ModelVertex> &polygon : obj.polygons) {
						bool skip_poly = false;
						std::vector<files::models_3d::ModelVertex> polygon_proj;
						std::vector<math::Vector> polygon_vectors;
						int i;
						bool is_outside;

						// Check if sun vec goes through
						// polygon

						// Project polygon points onto
						// sun plane
						for (files::models_3d::ModelVertex v : polygon) {
							math::Vector vert_as_vec, temp;

							// Move to sensor 0, 0, 0
							temp = math::Vector(v.x, v.y, v.z) - sun_sens.position;

							// Rotate sun vec to
							// align x axis
							temp = math::CoordinateSystems::vec_inertial_to_body({0,
							                                                      theta * 0.0174532925, psi * 0.0174532925},
							                                                     temp);
							// Here there is a bug,
							// if some points are
							// positive and some
							// negative it need to
							// be handled. Now it
							// skips polygon if at
							// least one is
							// negative.
							if (temp[0] < 0) {
								skip_poly = true;
								break;
							}

							vert_as_vec = temp.proj_onto_plane({1, 0, 0});

							polygon_proj.push_back({vert_as_vec[0], vert_as_vec[1], vert_as_vec[2]});
						}

						if (skip_poly)
							continue;

						// Get polygon vectors
						for (i = 0; i < polygon_proj.size(); ++i) {
							if (polygon_proj.size() - 1 == i) {
								polygon_vectors.push_back(
								    math::Vector(polygon_proj[0].x, polygon_proj[0].y, polygon_proj[0].z) -
								    math::Vector(polygon_proj[i].x, polygon_proj[i].y, polygon_proj[i].z));
								continue;
							}
							polygon_vectors.push_back(
							    math::Vector(polygon_proj[i + 1].x, polygon_proj[i + 1].y,
							                 polygon_proj[i + 1].z) -
							    math::Vector(polygon_proj[i].x, polygon_proj[i].y, polygon_proj[i].z));
						}

						is_outside = false;
						math::Vector norm2 = polygon_vectors[0].cross(
						    math::Vector({0, 0, 0}) -
						    math::Vector(polygon_proj[0].x, polygon_proj[0].y, polygon_proj[0].z));

						bool first_norm = norm2[0] > 0;
						for (uint8_t i = 1; i < polygon_vectors.size(); ++i) {
							math::Vector norm = polygon_vectors[i].cross(
							    math::Vector({0, 0, 0}) -
							    math::Vector(polygon_proj[i].x, polygon_proj[i].y, polygon_proj[i].z));
							bool current_norm = norm[0] > 0;
							if (current_norm != first_norm) {
								is_outside = true;
								break;
							}
						}

						if (!is_outside) {
							does_model_cover_sens = true;
							break;
						}
					}

					if (does_model_cover_sens)
						break;
				}

				sun_sens.is_on_sun[sun_sens.is_on_sun.size() - 1].push_back(does_model_cover_sens);
			}
		}
	}

	prog_bar.end();
}

void SimulationModelFile::_load_from_cache(uint32_t signature, storage::ICacheManager *cache_manager)
{
	int offset = 0, i, j, k;
	uint8_t num_of_sun_sens = 0, num_of_mag_sens;

	cache_manager->read("processed_models", std::to_string(signature), offset, &num_of_sun_sens, 1);
	offset += 1;
	cache_manager->read("processed_models", std::to_string(signature), offset, &num_of_mag_sens, 1);
	offset += 1;

	for (i = 0; i < num_of_sun_sens; ++i) {
		core::sensors::SunSensor sun_sensor;

		cache_manager->read("processed_models", std::to_string(signature), offset, (uint8_t *)&sun_sensor.position[0], 3 * 8);
		offset += 3 * 8;
		cache_manager->read("processed_models", std::to_string(signature), offset, (uint8_t *)&sun_sensor.orientation[0], 3 * 8);
		offset += 3 * 8;

		for (j = 0; j < 360; ++j) {
			sun_sensor.is_on_sun.push_back({});
			for (k = 0; k < 180; ++k) {
				uint8_t byt;

				cache_manager->read("processed_models", std::to_string(signature), offset, &byt, 1);
				sun_sensor.is_on_sun[j].push_back(byt);
			}
		}

		this->sun_sensors.push_back(sun_sensor);
	}

	for (i = 0; i < num_of_sun_sens; ++i) {
		core::sensors::MagnetometerSensor mag_sensor;

		cache_manager->read("processed_models", std::to_string(signature), offset, (uint8_t *)&mag_sensor.position[0], 3 * 8);
		offset += 3 * 8;
		cache_manager->read("processed_models", std::to_string(signature), offset, (uint8_t *)&mag_sensor.orientation[0], 3 * 8);
		offset += 3 * 8;

		this->magnetometer_sensors.push_back(mag_sensor);
	}
}

bool SimulationModelFile::_was_file_processed(uint32_t signature, storage::ICacheManager *cache_manager)
{
	return cache_manager->exists("processed_models", std::to_string(signature));
}

void SimulationModelFile::_mark_file_as_processed(const std::string &filepath, storage::ICacheManager *cache_manager)
{
	unsigned int signature = _get_file_signature(filepath);
	int offset;
	uint8_t num_of_sun_sensors, num_of_mag_sensors;

	if (cache_manager->exists("processed_models", std::to_string(signature)))
		cache_manager->clear("processed_models", std::to_string(signature));

	offset = 0;

	num_of_sun_sensors = this->sun_sensors.size();
	num_of_mag_sensors = this->magnetometer_sensors.size();

	cache_manager->write("processed_models", std::to_string(signature), offset, &num_of_sun_sensors, 1);
	offset += 1;
	cache_manager->write("processed_models", std::to_string(signature), offset, &num_of_mag_sensors, 1);
	offset += 1;

	for (const core::sensors::SunSensor &sun_sens : this->sun_sensors) {
		cache_manager->write("processed_models", std::to_string(signature), offset, (uint8_t *)&sun_sens.position[0], 3 * 8);
		offset += 3 * 8;
		cache_manager->write("processed_models", std::to_string(signature), offset, (uint8_t *)&sun_sens.orientation[0], 3 * 8);
		offset += 3 * 8;

		for (const std::vector<bool> &element : sun_sens.is_on_sun) {
			for (bool val : element) {
				uint8_t as_int = val ? 1 : 0;

				cache_manager->write("processed_models", std::to_string(signature), offset, &as_int, 1);
				offset += 1;
			}
		}
	}

	for (const core::sensors::MagnetometerSensor &mag_sens : this->magnetometer_sensors) {
		cache_manager->write("processed_models", std::to_string(signature), offset, (uint8_t *)&mag_sens.position[0], 3 * 8);
		offset += 3 * 8;
		cache_manager->write("processed_models", std::to_string(signature), offset, (uint8_t *)&mag_sens.orientation[0], 3 * 8);
		offset += 3 * 8;
	}
}

uint32_t SimulationModelFile::_get_file_signature(const std::string &filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	uint32_t signature;
	uint8_t i, ch;

	signature = 0;
	i = 0;

	do {
		ch = file.get();
		signature += ch * pow(2, i);
		++i;
		if (i == 8)
			i = 0;
	} while (!file.eof());

	file.close();

	return signature;
}

} // namespace file_processing
} // namespace sat_sim
