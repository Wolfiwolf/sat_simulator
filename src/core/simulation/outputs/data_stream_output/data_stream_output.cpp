#include <cstring>

#include "core/simulation_models/simulation_model.hpp"
#include "data_stream_output.hpp"
#include "data_streams/data_streams/idata_stream.hpp"

namespace sat_sim
{
namespace outputs
{

DataStreamOutput::DataStreamOutput(core::SimulationModel *sim_model, data_stream::IDataStream *data_stream)
    : IOutput(sim_model), _data_stream(data_stream)
{
}

DataStreamOutput::~DataStreamOutput()
{
}

void DataStreamOutput::output(uint64_t t)
{
	std::vector<uint8_t> bytes;
	uint8_t data[1024];

	data[0] = 0; // Required by protocol (indicates that packet is cubesat data)
	data[1] = t & 0xFF;
	data[2] = (t >> 8) & 0xFF;
	data[3] = (t >> 16) & 0xFF;
	data[4] = (t >> 24) & 0xFF;
	data[5] = (t >> 32) & 0xFF;
	data[6] = (t >> 40) & 0xFF;
	data[7] = (t >> 48) & 0xFF;
	data[8] = (t >> 56) & 0xFF;

	_sim_model->to_bytes(bytes);
	memcpy(data + 9, &bytes[0], bytes.size());

	_data_stream->send_bytes(data, bytes.size() + 9);
}

} // namespace outputs
} // namespace sat_sim
