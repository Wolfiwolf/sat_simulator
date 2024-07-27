#pragma once

#include <string>

#include "core/sensors/magnetometer_sensor/magnetometer_sensor.hpp"
#include "core/sensors/sun_sensor/sun_sensor.hpp"
#include "files/model_3d_file/model_3d_file.hpp"
#include "storage/cache_manager/icache_manager.hpp"

namespace sat_sim
{
namespace file_processing
{

class SimulationModelFile
{
private:
	bool _process_file(files::models_3d::Model3dFile *model_file);
	void _process_sun_sensors(files::models_3d::Model3dFile *model_file);

	bool _was_file_processed(uint32_t signature, storage::ICacheManager *cache_manager);
	void _load_from_cache(uint32_t signature, storage::ICacheManager *cache_manager);
	void _mark_file_as_processed(const std::string &filepath, storage::ICacheManager *cache_manager);
	uint32_t _get_file_signature(const std::string &filepath);

public:
	std::vector<core::sensors::SunSensor> sun_sensors;
	std::vector<core::sensors::MagnetometerSensor> magnetometer_sensors;

	bool load(files::models_3d::Model3dFile *model_file, storage::ICacheManager *cache_manager);
};

} // namespace file_processing
} // namespace sat_sim
