#pragma once

#include <stdint.h>
#include <string>

namespace sat_sim
{
namespace data_stream
{

class TCPClient
{
public:
	TCPClient();
	~TCPClient();

	int connect_to_server(const std::string &url, int port);

	void close_connection(int socket);

	bool send_data(int socket, uint8_t *data, uint16_t sz);

	void receive_data(int socket, uint8_t *buffer, uint16_t *sz);
};

} // namespace data_stream
} // namespace sat_sim
