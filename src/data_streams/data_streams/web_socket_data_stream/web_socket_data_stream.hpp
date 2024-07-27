#pragma once

#include <condition_variable>
#include <mutex>
#include <semaphore.h>
#include <vector>

#include "data_streams/data_streams/idata_stream.hpp"
#include "data_streams/web_socket_server/web_socket_server.hpp"

namespace sat_sim
{
namespace data_stream
{

class WebSocketDataStream : public IDataStream
{
private:
	WebSocketServer *_ws_server;
	bool _stop_flag;
	int _port;
	bool _is_connected;

	std::mutex _mtx;
	std::unique_lock<std::mutex> *_lock;
	std::condition_variable _cond_var;

	static void _listener(WebSocketDataStream *web_socket_data_stream);

public:
	WebSocketDataStream(int port);
	~WebSocketDataStream();

	bool start() override;

	void stop() override;

	bool is_up() override;

	void send_bytes(uint8_t *data, uint16_t sz) override;

	void receive_bytes(uint8_t *buffer, uint16_t *sz) override;
};

} // namespace data_stream
} // namespace sat_sim
