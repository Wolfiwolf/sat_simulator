#include <iostream>

#include "core/simulation_models/simulation_model.hpp"
#include "file_output.hpp"

namespace sat_sim
{
namespace outputs
{

FileOutput::FileOutput(core::SimulationModel *model) : IOutput(model)
{
	_file_out.open("log.bin", std::ios::out | std::ios::binary);
}

FileOutput::~FileOutput()
{
	_file_out.close();
}

void FileOutput::output(uint64_t t)
{
	std::vector<uint8_t> bytes;

	_sim_model->to_bytes(bytes);

	_file_out.write((char *)&t, 8);
	_file_out.write((char *)&bytes[0], bytes.size());
	_file_out.flush();
}

} // namespace outputs
} // namespace sat_sim
