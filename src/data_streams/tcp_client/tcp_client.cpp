#include <arpa/inet.h>
#include <exception>
#include <netdb.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "debug/logger/logger.hpp"
#include "tcp_client.hpp"

namespace sat_sim
{
namespace data_stream
{

TCPClient::TCPClient()
{
}

TCPClient::~TCPClient()
{
}

int TCPClient::connect_to_server(const std::string &url, int port)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		debug::Logger::Log("TCPClient: Creating socket failed", debug::LogLevel::DANGER);
		return -1;
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(url.c_str());
	servaddr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		debug::Logger::Log("TCPClient: connecting failed", debug::LogLevel::DANGER);
		return -1;
	}

	return sockfd;
}

void TCPClient::close_connection(int socket)
{
	close(socket);
}

bool TCPClient::send_data(int socket, uint8_t *data, uint16_t sz)
{
	int res = send(socket, data, sz, 0);

	return res != -1;
}

void TCPClient::receive_data(int socket, uint8_t *buffer, uint16_t *sz)
{
	*sz = recv(socket, buffer, 4096, 0);
}

} // namespace data_stream
} // namespace sat_sim
