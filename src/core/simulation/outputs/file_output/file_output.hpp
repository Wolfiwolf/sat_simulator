#pragma once

#include <fstream>

#include "../ioutput.hpp"
#include "core/simulation_models/simulation_model.hpp"

namespace sat_sim
{
namespace outputs
{

class FileOutput : public IOutput
{
private:
	std::ofstream _file_out;

public:
	FileOutput(core::SimulationModel *model);

	~FileOutput();

	void output(uint64_t t) override;
};

} // namespace outputs
} // namespace sat_sim
