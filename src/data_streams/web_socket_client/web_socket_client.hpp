#pragma once

#include <string>

#include "data_streams/tcp_client/tcp_client.hpp"

namespace sat_sim
{
namespace data_stream
{

class WebSocketClient
{
private:
	TCPClient *_tcp_client;

	std::string _base64_encode(const uint8_t *data, size_t size);
	std::string _generate_key();

public:
	WebSocketClient();
	~WebSocketClient();

	int connect(const std::string &url, int port);

	void close(int server_id);

	void send(int server_id, uint8_t *data, uint32_t data_len);

	void receive(int server_id, uint8_t *buffer, uint8_t *buffer_len);
};

} // namespace data_stream
} // namespace sat_sim
