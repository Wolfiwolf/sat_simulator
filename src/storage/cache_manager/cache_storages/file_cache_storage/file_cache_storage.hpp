#pragma once

#include <fstream>
#include <unordered_map>

#include "../icache_storage.hpp"
#include "system/system_info/isystem_info.hpp"

namespace sat_sim
{
namespace storage
{

class FileCacheStorage : public ICacheStorage
{
private:
	system::ISystemInfo *_system_info;
	std::string _cache_path;
	std::unordered_map<std::string, FILE *> _opened_caches;

public:
	FileCacheStorage(system::ISystemInfo *system_info);
	~FileCacheStorage();

	void init();

	void write_to(const std::string &department, const std::string &key, uint32_t address, uint8_t *data, uint32_t sz);

	bool read_from(const std::string &department, const std::string &key, uint32_t address, uint8_t *buffer, uint32_t sz);

	void clear(const std::string &department, const std::string &key);

	bool exists(const std::string &department, const std::string &key);
};

} // namespace storage
} // namespace sat_sim
