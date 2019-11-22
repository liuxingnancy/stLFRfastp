#ifndef  THREAD_SAVE_MAP_H
#define THREAD_SAVE_MAP_H

#include <map>
#include <memory>
#include<mutex>

using namespace std;

template<typename Tkey, typename Tvalue>
class threadsavemap{
public:
	threadsavemap();
	~threadsavemap();
	bool insert(const Tkey& key, const Tvalue& value, bool cover = false);
	void remove(const Tkey& key);
	bool lookup(const Tkey& key, Tvalue& value);
	int size();

private:
	std::mutex m_mutextMap;
	std::map<Tkey, Tvalue> m_map;
};

#endif // ! THREAD_SAVE_MAP_H
