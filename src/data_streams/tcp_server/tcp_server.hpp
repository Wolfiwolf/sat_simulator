#pragma once

#include <mutex>
#include <stdint.h>

namespace sat_sim
{
namespace data_stream
{

class TCPServer
{
private:
	int _port;
	int _connection;
	int _listening_socket;
	std::mutex _connection_mtx;
	std::mutex _recv_mtx;

public:
	TCPServer(int port);

	void stop();

	void wait_for_connection();

	void receive_bytes(uint8_t *buffer, uint16_t *sz);

	void send_bytes(uint8_t *buffer, uint16_t sz);
};

} // namespace data_stream
} // namespace sat_sim
