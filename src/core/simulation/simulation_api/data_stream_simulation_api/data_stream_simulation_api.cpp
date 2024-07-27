#include <string.h>

#include "data_stream_simulation_api.hpp"

namespace sat_sim
{
namespace core
{

DataStreamSimulationApi::DataStreamSimulationApi(data_stream::IDataStream *data_stream)
{
	_data_stream = data_stream;
}

DataStreamSimulationApi::Request DataStreamSimulationApi::_get_request()
{
	Request req;
	uint8_t buffer[1025];
	uint16_t buffer_sz;

	_data_stream->receive_bytes(buffer, &buffer_sz);

	if (buffer_sz == 0)
		req.type = RequestType::FAIL;
	else {
		req.type = (RequestType)buffer[0];
		req.data_sz = buffer_sz - 1;
		memcpy(req.data, buffer + 1, req.data_sz);
	}

	return req;
}

void DataStreamSimulationApi::_send_response(const Response &res)
{
	uint8_t buffer[1024 + 1];

	buffer[0] = 1;
	memcpy(buffer + 1, res.data, res.data_sz);

	_data_stream->send_bytes(buffer, res.data_sz + 1);
}

} // namespace core
} // namespace sat_sim
