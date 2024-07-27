#pragma once

#include <stdint.h>

namespace sat_sim
{
namespace data_stream
{

class IDataStream
{
public:
	virtual inline ~IDataStream()
	{
	}

	virtual bool start() = 0;

	virtual void stop() = 0;

	virtual bool is_up() = 0;

	virtual void send_bytes(uint8_t *data, uint16_t sz) = 0;

	virtual void receive_bytes(uint8_t *buffer, uint16_t *sz) = 0;
};

} // namespace data_stream
} // namespace sat_sim
