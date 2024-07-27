#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "data_streams/tcp_client/tcp_client.hpp"
#include "web_socket_client.hpp"

namespace sat_sim
{
namespace data_stream
{

WebSocketClient::WebSocketClient()
{
	_tcp_client = new TCPClient();
}

WebSocketClient::~WebSocketClient()
{
	delete _tcp_client;
}

int WebSocketClient::connect(const std::string &url, int port)
{
	std::string ws_key, handshake_msg;
	int server_id;
	uint16_t buffer_size;
	uint8_t buffer[4096];

	ws_key = _generate_key();

	handshake_msg = "GET / HTTP/1.1\r\n";
	handshake_msg += "Host: localhost:8909\r\n";
	handshake_msg += "Upgrade: websocket\r\n";
	handshake_msg += "Connection: Upgrade\r\n";
	handshake_msg += "Sec-WebSocket-Key: " + ws_key + "\r\n";
	handshake_msg += "Sec-WebSocket-Version: 13\r\n\r\n";

	server_id = _tcp_client->connect_to_server(url, port);

	_tcp_client->send_data(server_id, (uint8_t *)handshake_msg.c_str(), handshake_msg.size());

	_tcp_client->receive_data(server_id, buffer, &buffer_size);

	return server_id;
}

void WebSocketClient::close(int server_id)
{
	_tcp_client->close_connection(server_id);
}

void WebSocketClient::send(int server_id, uint8_t *data, uint32_t data_len)
{
	std::vector<uint8_t> frame;
	uint32_t maskingKey;
	int i;

	// HEADER
	frame.push_back(0x80 | 2);
	if (data_len < 126) {
		frame.push_back(0x80 | data_len);
	} else {
		frame.push_back(0x80 | 126);
		frame.push_back((data_len >> 8) & 0xFF);
		frame.push_back(data_len & 0xFF);
	}

	// MASKING KEY
	maskingKey = 0x89415678;
	for (i = 0; i < 4; ++i)
		frame.push_back((maskingKey >> (i * 8)) & 0xFF);

	// PAYLOAD
	for (i = 0; i < data_len; ++i)
		frame.push_back(data[i] ^ ((maskingKey >> ((i % 4) * 8)) & 0xFF));

	_tcp_client->send_data(server_id, (uint8_t *)&frame[0], frame.size());
}

void WebSocketClient::receive(int server_id, uint8_t *buffer, uint8_t *buffer_len)
{
	uint16_t msg_buffer_len;
	uint8_t msg_buffer[4069];

	_tcp_client->receive_data(server_id, msg_buffer, &msg_buffer_len);
	memcpy(buffer, msg_buffer + 2, msg_buffer_len - 2);
	*buffer_len = (msg_buffer_len & 0xFF) - 2;
}

std::string WebSocketClient::_base64_encode(const uint8_t *data, size_t size)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string encoded;
	int i, j;

	for (i = 0; i < size; i += 3) {
		uint32_t group = 0;

		for (j = 0; j < 3; ++j) {
			group <<= 8;
			if (i + j < size) {
				group |= data[i + j];
			}
		}

		for (j = 0; j < 4; ++j) {
			if (i + j <= size) {
				encoded += base64_chars[(group >> (18 - j * 6)) & 0x3F];
			} else {
				encoded += '=';
			}
		}
	}

	return encoded;
}

std::string WebSocketClient::_generate_key()
{
	uint8_t data[16];
	int i;

	for (i = 0; i < 16; ++i)
		data[i] = rand() % 256;

	return _base64_encode(data, 16);
}

} // namespace data_stream
} // namespace sat_sim
