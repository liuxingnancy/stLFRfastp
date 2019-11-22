#include "threadsavemap.h"

template<typename Tkey, typename Tvalue>
threadsavemap<Tkey, Tvalue>::threadsavemap()
{
}

template<typename Tkey, typename Tvalue>
threadsavemap<Tkey, Tvalue>::~threadsavemap()
{
	std::lock_guard<std::mutex> locker(m_mutextMap);
	m_map.clear();
}

template<typename Tkey, typename Tvalue>
bool threadsavemap<Tkey, Tvalue>::insert(const Tkey& key, const Tvalue& value, bool cover)
{
	std::lock_guard<std::mutex> locker(m_mutextMap);
	auto find = m_map.find(key);
	if (find != m_map.end() && cover) {
		m_map.erase(find);
	}
	auto result = m_map.insert(std::pair<Tkey, Tvalue>(key, value));
	return result.second;
}

template<typename Tkey, typename Tvalue>
void threadsavemap<Tkey, Tvalue>::remove(const Tkey& key)
{
	std::lock_guard<std::mutex> locker(m_mutextMap);
	auto find = m_map.find(key);
	if (find != m_map.end()) {
		m_map.erase(find);
	}
}

template<typename Tkey, typename Tvalue>
bool threadsavemap<Tkey, Tvalue>::lookup(const Tkey& key, Tvalue& value)
{
	std::lock_guard<std::mutex> locker(m_mutextMap);
	auto find = m_map.find(key);
	return false;
}

template<typename Tkey, typename Tvalue>
int threadsavemap<Tkey, Tvalue>::size()
{
	std::lock_guard<std::mutex> locker(m_mutextMap);
	return m_map.size();
}

