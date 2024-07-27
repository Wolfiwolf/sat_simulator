#pragma once

#include <mutex>
#include <stdint.h>

#include "data_streams/tcp_server/tcp_server.hpp"

namespace sat_sim
{
namespace data_stream
{

class WebSocketServer
{
public:
	WebSocketServer(int port);

	~WebSocketServer();

	void stop();

	bool wait_for_connection();

	void receive_bytes(uint8_t *buffer, uint16_t *sz);

	void send_bytes(uint8_t *buffer, uint16_t sz);

private:
	TCPServer *_tcp_server;

	static void _sha1(std::string input, uint8_t hash[20]);

	static std::string _get_key_from_request(const std::string &request);

	static std::string _base64_encode(const uint8_t *data, size_t size);
};

} // namespace data_stream
} // namespace sat_sim
