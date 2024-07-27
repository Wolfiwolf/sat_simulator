#include <cstring>
#include <iostream>
#include <thread>

#include "simulation_api.hpp"

namespace sat_sim
{
namespace core
{

SimulationApi::~SimulationApi()
{
}

void SimulationApi::wait_and_handle_request()
{
	Request req = _get_request();
	bool result = true;
	RequestType request_type;
	Response response;

	response.data_sz = 0;

	request_type = (RequestType)req.type;

	switch (request_type) {
	case RequestType::CONNECTION_TEST:
		result = _request_connection_test_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	case RequestType::PAUSE:
		result = _request_pause_callback();
		break;
	case RequestType::UNPAUSE:
		result = _request_unpause_callback();
		break;
	case RequestType::STOP:
		result = _request_stop_callback();
		break;
	case RequestType::ECI_TO_ECEF:
		result = _request_eci_to_ecef_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	case RequestType::ECEF_TO_ECI:
		result = _request_ecef_to_eci_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	case RequestType::ECEF_TO_GEODETIC:
		result = _request_ecef_to_geodetic_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	case RequestType::GET_SUN_VEC:
		result = _request_get_sun_vec_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	case RequestType::GET_MAG_VEC:
		result = _request_get_mag_vec_callback(req.data, req.data_sz, response.data, &response.data_sz);
		break;
	default:
		result = false;
		break;
	}

	if (!result) {
		response.data[0] = ':';
		response.data[1] = '(';
		response.data_sz = 2;
	}

	if (response.data_sz == 0) {
		response.data[0] = ':';
		response.data[1] = ')';
		response.data_sz = 2;
	}

	if (response.data_sz <= 1024)
		_send_response(response);
}

void SimulationApi::set_request_connection_test_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_connection_test_callback = callback;
}

void SimulationApi::set_request_pause_callback(std::function<bool()> callback)
{
	_request_pause_callback = callback;
}

void SimulationApi::set_request_unpause_callback(std::function<bool()> callback)
{
	_request_unpause_callback = callback;
}

void SimulationApi::set_request_stop_callback(std::function<bool()> callback)
{
	_request_stop_callback = callback;
}

void SimulationApi::set_request_eci_to_ecef_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_eci_to_ecef_callback = callback;
}

void SimulationApi::set_request_ecef_to_eci_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_ecef_to_eci_callback = callback;
}

void SimulationApi::set_request_ecef_to_geodetic_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_ecef_to_geodetic_callback = callback;
}

void SimulationApi::set_request_get_sun_vec_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_get_sun_vec_callback = callback;
}

void SimulationApi::set_request_get_mag_vec_callback(
    std::function<bool(uint8_t *data, uint16_t data_len, uint8_t *buffer, uint16_t *buffer_len)> callback)
{
	_request_get_mag_vec_callback = callback;
}

} // namespace core
} // namespace sat_sim
