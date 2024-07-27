#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "core/sensors/sensor.hpp"
#include "math/vector/vector.hpp"

namespace sat_sim
{
namespace core
{
namespace sensors
{

class SunSensor : public Sensor
{
private:
	double _val;

	static std::function<double(double, const math::Vector &)> _transfer_function;

public:
	static void init();

	SunSensor();

	~SunSensor();

	std::vector<std::vector<bool>> is_on_sun;

	void update(uint64_t t_unix, double delta_t) override;

	double get_val();
};

} // namespace sensors
} // namespace core
} // namespace sat_sim
