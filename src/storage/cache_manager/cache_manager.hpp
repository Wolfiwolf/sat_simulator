#pragma once

#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/cache_manager/cache_storages/icache_storage.hpp"
#include "storage/cache_manager/icache_manager.hpp"

namespace sat_sim
{
namespace storage
{

class CacheManager : public ICacheManager
{
private:
	std::mutex _mtx;
	ICacheStorage *_cache_storage;

public:
	void init(ICacheStorage *cache_storage);

	void end();

	void write(const std::string &department, const std::string &key, uint32_t address, uint8_t *data, uint32_t sz);

	void read(const std::string &department, const std::string &key, uint32_t address, uint8_t *data, uint32_t sz);

	void clear(const std::string &department, const std::string &key);

	bool exists(const std::string &department, const std::string &key);
};

} // namespace storage
} // namespace sat_sim
