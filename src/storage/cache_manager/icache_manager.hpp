#pragma once

#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/cache_manager/cache_storages/icache_storage.hpp"

namespace sat_sim
{
namespace storage
{

class ICacheManager
{
public:
	virtual void init(ICacheStorage *cache_storage) = 0;

	virtual void end() = 0;

	virtual void write(const std::string &department, const std::string &key,
	                   uint32_t address, uint8_t *data, uint32_t sz) = 0;

	virtual void read(const std::string &department, const std::string &key,
	                  uint32_t address, uint8_t *data, uint32_t sz) = 0;

	virtual void clear(const std::string &department, const std::string &key) = 0;

	virtual bool exists(const std::string &department, const std::string &key) = 0;
};

} // namespace storage
} // namespace sat_sim
