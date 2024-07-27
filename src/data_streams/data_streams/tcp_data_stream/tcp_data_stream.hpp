#pragma once

#include "data_streams/data_streams/idata_stream.hpp"
#include "data_streams/tcp_server/tcp_server.hpp"

namespace sat_sim
{
namespace data_stream
{

class TCPDataStream : public IDataStream
{
private:
	TCPServer *_tcp_server;
	bool _stop_flag;
	int _port;
	bool _is_running;

	static void _listener(TCPDataStream *tcp_data_stream);

public:
	TCPDataStream(int port);
	~TCPDataStream();

	bool start() override;

	void stop() override;

	bool is_up() override;

	void send_bytes(uint8_t *data, uint16_t sz) override;

	void receive_bytes(uint8_t *buffer, uint16_t *sz) override;
};

} // namespace data_stream
} // namespace sat_sim
