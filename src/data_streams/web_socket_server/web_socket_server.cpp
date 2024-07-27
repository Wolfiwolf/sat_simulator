#include <sstream>
#include <string.h>
#include <unordered_map>
#include <vector>

#include "data_streams/tcp_server/tcp_server.hpp"
#include "external/sha1/sha1.hpp"
#include "web_socket_server.hpp"

namespace sat_sim
{
namespace data_stream
{

WebSocketServer::WebSocketServer(int port)
{
	_tcp_server = new TCPServer(port);
}

WebSocketServer::~WebSocketServer()
{
	delete _tcp_server;
}

void WebSocketServer::stop()
{
	_tcp_server->stop();
}

bool WebSocketServer::wait_for_connection()
{
	uint8_t buffer[1024];
	uint16_t len;

	_tcp_server->wait_for_connection();
	_tcp_server->receive_bytes(buffer, &len);

	if (len != 0) {
		std::string req((const char *)buffer, len);
		std::string key = _get_key_from_request(req);
		std::string to_hash = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		std::string to_send, response;
		uint8_t hashed[20];

		_sha1(to_hash, hashed);
		to_send = _base64_encode(hashed, 20);

		response = "HTTP/1.1 101 Switching Protocols\r\n";
		response += "Sec-WebSocket-Accept: " + to_send + "\r\n";
		response += "Upgrade: websocket\r\n";
		response += "Connection: Upgrade\r\n\r\n";

		_tcp_server->send_bytes((uint8_t *)response.c_str(), response.size());

		return true;
	}

	return false;
}

void WebSocketServer::receive_bytes(uint8_t *buffer, uint16_t *sz)
{
	uint16_t frame_size;
	uint8_t mask[4], data[256], frame_buffer[64];
	int i;

	_tcp_server->receive_bytes(frame_buffer, &frame_size);
	if (frame_size == 0) {
		*sz = 0;
		return;
	}

	*sz = frame_buffer[1] & 0b01111111;
	memcpy(mask, frame_buffer + 2, 4);

	memcpy(data, frame_buffer + (frame_size - *sz), *sz);
	for (i = 0; i < *sz; ++i)
		buffer[i] = data[i] ^ mask[i % 4];
}

void WebSocketServer::send_bytes(uint8_t *buffer, uint16_t sz)
{
	std::vector<uint8_t> frame;
	int i;

	// HEADER
	frame.push_back(0x80 | 2);
	if (sz < 126) {
		frame.push_back(sz);
	} else {
		frame.push_back(126);
		frame.push_back((sz >> 8) & 0xFF);
		frame.push_back(sz & 0xFF);
	}

	// PAYLOAD
	for (i = 0; i < sz; ++i)
		frame.push_back(buffer[i]);

	_tcp_server->send_bytes((uint8_t *)&frame[0], frame.size());
}

std::string WebSocketServer::_base64_encode(const uint8_t *data, size_t size)
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

std::string WebSocketServer::_get_key_from_request(const std::string &request)
{
	std::stringstream ss(request);
	std::string line;

	std::getline(ss, line);
	while (std::getline(ss, line)) {
		int index = line.find("Sec-WebSocket-Key");
		std::string key;

		if (index == std::string::npos)
			continue;

		key = line.substr(19, line.size() - 20);

		return key;
	}

	return "";
}

void WebSocketServer::_sha1(std::string input, uint8_t hash[20])
{
	static std::unordered_map<char, uint8_t> hex_table = {
	    {'0', 0},
	    {'1', 1},
	    {'2', 2},
	    {'3', 3},
	    {'4', 4},
	    {'5', 5},
	    {'6', 6},
	    {'7', 7},
	    {'8', 8},
	    {'9', 9},
	    {'a', 10},
	    {'b', 11},
	    {'c', 12},
	    {'d', 13},
	    {'e', 14},
	    {'f', 15}};
	SHA1 sha1;
	std::string as_str;
	int i;

	sha1.update(input);
	as_str = sha1.final();

	for (i = 0; i < 40; i += 2) {
		hash[i / 2] = hex_table[as_str[i]] << 4;
		hash[i / 2] |= hex_table[as_str[i + 1]];
	}
}
} // namespace data_stream
} // namespace sat_sim
