#pragma once

#include <functional>
#include <stdint.h>

namespace sat_sim
{
namespace core
{

class SimulationApi
{
protected:
	enum class RequestType {
		FAIL = 0xFF,
		CONNECTION_TEST = 0,
		PAUSE,
		UNPAUSE,
		STOP,
		ECI_TO_ECEF,
		ECEF_TO_ECI,
		ECEF_TO_GEODETIC,
		GET_SUN_VEC,
		GET_MAG_VEC
	};

	struct Request {
		RequestType type;
		uint8_t data[1024];
		uint16_t data_sz;
	};

	struct Response {
		uint8_t data[1024];
		uint16_t data_sz;
	};

	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_connection_test_callback;
	std::function<bool()> _request_pause_callback;
	std::function<bool()> _request_unpause_callback;
	std::function<bool()> _request_stop_callback;
	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_eci_to_ecef_callback;
	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_ecef_to_eci_callback;
	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_ecef_to_geodetic_callback;
	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_get_sun_vec_callback;
	std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> _request_get_mag_vec_callback;

	virtual Request _get_request() = 0;

	virtual void _send_response(const Response &res) = 0;

public:
	virtual ~SimulationApi();

	void wait_and_handle_request();

	void set_request_connection_test_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);

	void set_request_pause_callback(std::function<bool()> callback);

	void set_request_unpause_callback(std::function<bool()> callback);

	void set_request_stop_callback(std::function<bool()> callback);

	void set_request_eci_to_ecef_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);

	void set_request_ecef_to_eci_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);

	void set_request_ecef_to_geodetic_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);

	void set_request_get_sun_vec_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);

	void set_request_get_mag_vec_callback(
	    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback);
};

} // namespace core
} // namespace sat_sim
