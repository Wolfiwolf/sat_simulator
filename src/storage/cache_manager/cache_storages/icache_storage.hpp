#pragma once

#include <stdint.h>
#include <string>

namespace sat_sim
{
namespace storage
{

class ICacheStorage
{
public:
	virtual inline ~ICacheStorage()
	{
	}

	virtual void init() = 0;

	virtual void write_to(const std::string &department, const std::string &key, uint32_t address, uint8_t *data, uint32_t sz) = 0;

	virtual bool read_from(const std::string &department, const std::string &key, uint32_t address, uint8_t *buffer, uint32_t sz) = 0;

	virtual void clear(const std::string &department, const std::string &key) = 0;

	virtual bool exists(const std::string &department, const std::string &key) = 0;
};

} // namespace storage
} // namespace sat_sim
