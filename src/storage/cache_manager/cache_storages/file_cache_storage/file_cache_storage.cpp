#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <unistd.h>

#include "file_cache_storage.hpp"
#include "system/system_info/isystem_info.hpp"

namespace sat_sim
{
namespace storage
{

FileCacheStorage::FileCacheStorage(system::ISystemInfo *system_info)
    : _system_info(system_info)
{
}

FileCacheStorage::~FileCacheStorage()
{
}

void FileCacheStorage::init()
{
	_cache_path = "/home/" + _system_info->get_user_name() + "/.cache/sat_sim";
}

void FileCacheStorage::write_to(const std::string &department, const std::string &key, uint32_t address,
                                uint8_t *data, uint32_t sz)
{
	std::string real_key = department + "/" + key;
	int file_sz;

	if (_opened_caches.find(real_key) == _opened_caches.end()) {
		if (!std::filesystem::exists(_cache_path)) {
			std::filesystem::create_directory(_cache_path);
			if (!std::filesystem::exists(_cache_path + "/" + department))
				std::filesystem::create_directory(_cache_path + "/" + department);
		}

		if (!std::filesystem::exists(_cache_path + "/" + real_key)) {
			FILE *temp_f = fopen(std::string(_cache_path + "/" + real_key).c_str(), "w");

			fclose(temp_f);
		}

		_opened_caches[real_key] = fopen(std::string(_cache_path + "/" + real_key).c_str(), "r+");
	}

	fseek(_opened_caches[real_key], 0, SEEK_END);
	file_sz = ftell(_opened_caches[real_key]);

	if (file_sz < address + sz) {
		int i, to_add;
		uint8_t zero_num = 0;

		to_add = (address + sz) - file_sz;

		for (i = 0; i < to_add; ++i)
			fwrite(&zero_num, 1, 1, _opened_caches[real_key]);
	}

	fseek(_opened_caches[real_key], address, SEEK_SET);
	fwrite(data, 1, sz, _opened_caches[real_key]);
}

bool FileCacheStorage::read_from(const std::string &department, const std::string &key,
                                 uint32_t address, uint8_t *buffer, uint32_t sz)
{
	std::string real_key = department + "/" + key;

	if (_opened_caches.find(real_key) == _opened_caches.end()) {
		if (!std::filesystem::exists(_cache_path + "/" + department))
			return false;

		_opened_caches[real_key] = fopen(std::string(_cache_path + "/" + real_key).c_str(), "r+");
	}

	fseek(_opened_caches[real_key], address, SEEK_SET);
	fread(buffer, 1, sz, _opened_caches[real_key]);

	return true;
}

void FileCacheStorage::clear(const std::string &department, const std::string &key)
{
	std::string path = _cache_path + "/" + department + "/" + key;

	if (std::filesystem::exists(path))
		std::filesystem::remove(path);
}

bool FileCacheStorage::exists(const std::string &department, const std::string &key)
{
	std::string path = _cache_path + "/" + department + "/" + key;

	return std::filesystem::exists(path);
}

} // namespace storage
} // namespace sat_sim
