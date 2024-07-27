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

class MagnetometerSensor : public Sensor
{
private:
	math::Vector _val;

	static std::function<math::Vector(double, const math::Vector &)> _transfer_function;

public:
	static void init();

	MagnetometerSensor();

	~MagnetometerSensor();

	void update(uint64_t t_unix, double delta_t) override;

	math::Vector get_val();
};

} // namespace sensors
} // namespace core
} // namespace sat_sim
