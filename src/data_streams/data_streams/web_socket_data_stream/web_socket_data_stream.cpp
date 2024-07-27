#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <netinet/in.h>
#include <signal.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/cdefs.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <iostream>

#include "data_streams/web_socket_server/web_socket_server.hpp"
#include "web_socket_data_stream.hpp"

namespace sat_sim
{
namespace data_stream
{

WebSocketDataStream::WebSocketDataStream(int port)
{
	_port = port;
	_is_connected = false;
	_ws_server = new WebSocketServer(port);
	_lock = new std::unique_lock(_mtx);
}

WebSocketDataStream::~WebSocketDataStream()
{
	delete _ws_server;
	delete _lock;
}

bool WebSocketDataStream::start()
{
	_stop_flag = false;

	std::thread task(_listener, this);

	task.detach();

	return true;
}

void WebSocketDataStream::stop()
{
	_stop_flag = true;
	_cond_var.notify_all();
	_ws_server->stop();
}

bool WebSocketDataStream::is_up()
{
	return !_stop_flag;
}

void WebSocketDataStream::receive_bytes(uint8_t *buffer, uint16_t *sz)
{
	if (_is_connected == false)
		_cond_var.wait(*_lock);

	_ws_server->receive_bytes(buffer, sz);

	if (*sz == 0) {
		_is_connected = false;
		_cond_var.notify_all();
	}
}

void WebSocketDataStream::send_bytes(uint8_t *data, uint16_t sz)
{
	if (_is_connected == false)
		return;
	_ws_server->send_bytes(data, sz);
}

void WebSocketDataStream::_listener(WebSocketDataStream *web_socket_data_stream)
{
	web_socket_data_stream->_stop_flag = false;

	while (!web_socket_data_stream->_stop_flag) {
		if (web_socket_data_stream->_is_connected)
			web_socket_data_stream->_cond_var.wait(*(web_socket_data_stream->_lock));

		web_socket_data_stream->_ws_server->wait_for_connection();
		std::cout << "New connection\n";

		web_socket_data_stream->_is_connected = true;
		web_socket_data_stream->_cond_var.notify_all();
	}
}
} // namespace data_stream
} // namespace sat_sim
