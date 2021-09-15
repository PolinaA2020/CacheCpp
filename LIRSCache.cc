// LIRS Cache algorithm c++ realizaition 
#include <iostream>
#include <stdio.h>

template <typename T, typename KeyT = int>
struct cache_t {
	size_t size_;
	std::list<T> cache_;
	std::unordered_map<KeyT, cache_> LIRHash_;
	std::unordered_map<KeyT, cache_> HIRHash_;



	bool lirsCacheLookUp(KeyT key, slow_get_page);
};

bool lirsCacheLookUp(KeyT key, slow_get_page)
{
	auto hit = LIRHash_.find(key); // check if key is in LIRS
	if(!hit)
		hit = HIRHash_.find(key); // check if key is in HIRS
	


}

