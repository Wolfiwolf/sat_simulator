#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/cdefs.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "data_streams/tcp_client/tcp_client.hpp"
#include "debug/logger/logger.hpp"
#include "tcp_server.hpp"

namespace sat_sim
{
namespace data_stream
{

TCPServer::TCPServer(int port)
{
	sockaddr_in hint;
	int temp;

	_port = port;
	_connection = -1;
	_recv_mtx.lock();
	_listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listening_socket == -1) {
		debug::Logger::Log("Can't create a socket! Quitting", debug::LogLevel::DANGER);
		return;
	}

	temp = 1;
	setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, &temp, 4);

	hint.sin_family = AF_INET;
	hint.sin_port = htons(_port);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	if (bind(_listening_socket, (sockaddr *)&hint, sizeof(hint)) != 0) {
		debug::Logger::Log("Binding failed", debug::LogLevel::DANGER);
		return;
	}

	listen(_listening_socket, SOMAXCONN);
}

void TCPServer::stop()
{
	TCPClient client;
	int sock;

	close(_listening_socket);

	sock = client.connect_to_server("127.0.0.1", 5050);
	client.close_connection(sock);
}

void TCPServer::send_bytes(uint8_t *data, uint16_t sz)
{
	if (_connection != -1)
		send(_connection, data, sz, 0);
}

void TCPServer::receive_bytes(uint8_t *buffer, uint16_t *sz)
{
	if (_connection == -1)
		_recv_mtx.lock();
	*sz = recv(_connection, buffer, 4096, 0);
}

void TCPServer::wait_for_connection()
{
	sockaddr_in client;
	socklen_t client_size = sizeof(client);
	int client_socket = accept(_listening_socket, (sockaddr *)&client, &client_size);

	if (client_socket < 0)
		return;

	_connection_mtx.lock();
	_connection = client_socket;
	_connection_mtx.unlock();

	_recv_mtx.unlock();
}

} // namespace data_stream
} // namespace sat_sim
