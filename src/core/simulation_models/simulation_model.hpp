#pragma once

#include <stdint.h>
#include <vector>

namespace sat_sim
{
namespace core
{

class SimulationModel
{
public:
	virtual void update(uint64_t t, double delta_t) = 0;

	virtual void to_bytes(std::vector<uint8_t> &buffer) = 0;
};

} // namespace core
} // namespace sat_sim
