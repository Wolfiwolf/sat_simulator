#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <csignal>
#include <iomanip>
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/cdefs.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>

#include "data_streams/tcp_server/tcp_server.hpp"
#include "tcp_data_stream.hpp"

namespace sat_sim
{
namespace data_stream
{

TCPDataStream::TCPDataStream(int port)
    : _port(port), _stop_flag(false), _is_running(false)
{
	_port = port;
	_tcp_server = new TCPServer(port);

	_stop_flag = false;
	_is_running = false;
}

TCPDataStream::~TCPDataStream()
{
}

bool TCPDataStream::start()
{
	std::thread task(_listener, this);

	task.detach();

	return true;
}

void TCPDataStream::stop()
{
	_stop_flag = true;
	_tcp_server->stop();
}

bool TCPDataStream::is_up()
{
	return _is_running;
}

void TCPDataStream::receive_bytes(uint8_t *buffer, uint16_t *sz)
{
	_tcp_server->receive_bytes(buffer, sz);
}

void TCPDataStream::send_bytes(uint8_t *data, uint16_t sz)
{
	_tcp_server->send_bytes(data, sz);
}

void TCPDataStream::_listener(TCPDataStream *tcp_data_stream)
{
	tcp_data_stream->_is_running = true;

	while (!tcp_data_stream->_stop_flag) {
		tcp_data_stream->_tcp_server->wait_for_connection();
	}
	tcp_data_stream->_is_running = false;
}

} // namespace data_stream
} // namespace sat_sim
