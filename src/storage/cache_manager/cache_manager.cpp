#include "cache_manager.hpp"

namespace sat_sim
{
namespace storage
{

void CacheManager::init(ICacheStorage *cache_storage)
{
	_cache_storage = cache_storage;
	_cache_storage->init();
}

void CacheManager::end()
{
	delete _cache_storage;
}

void CacheManager::write(const std::string &department, const std::string &key,
                         uint32_t address, uint8_t *data, uint32_t sz)
{
	_mtx.lock();
	_cache_storage->write_to(department, key, address, data, sz);
	_mtx.unlock();
}

void CacheManager::read(const std::string &department, const std::string &key,
                        uint32_t address, uint8_t *data, uint32_t sz)
{
	_mtx.lock();
	_cache_storage->read_from(department, key, address, data, sz);
	_mtx.unlock();
}

void CacheManager::clear(const std::string &department, const std::string &key)
{
	_mtx.lock();
	_cache_storage->clear(department, key);
	_mtx.unlock();
}

bool CacheManager::exists(const std::string &department, const std::string &key)
{
	bool res;

	_mtx.lock();
	res = _cache_storage->exists(department, key);
	_mtx.unlock();

	return res;
}
} // namespace storage
} // namespace sat_sim
