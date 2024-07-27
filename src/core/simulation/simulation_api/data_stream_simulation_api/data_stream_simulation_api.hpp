#pragma once

#include <functional>
#include <stdint.h>

#include "core/simulation/simulation_api/simulation_api.hpp"
#include "data_streams/data_streams/idata_stream.hpp"

namespace sat_sim
{
namespace core
{

class DataStreamSimulationApi : public SimulationApi
{
private:
	data_stream::IDataStream *_data_stream;

	Request _get_request() override;

	void _send_response(const Response &res) override;

public:
	DataStreamSimulationApi(data_stream::IDataStream *data_stream);
};

} // namespace core
} // namespace sat_sim
