// main for LIRSCache
#include "LIRSCache.hpp"
#include <cstring>
#include <cassert>
#include <fstream>

enum TypeOfValue {
	TYPE_OF_VALUE_INT,
	TYPE_OF_VALUE_FLOAT,
	TYPE_OF_VALUE_STRING,
	TYPE_OF_VALUE_ERORR,
};

const std::string typeOfValueArr[] = {"int", "float", "string"};
/*============================================================*/
int getTypeOfValue(const std::string type_str)
{
	for(int i = 0; i < TYPE_OF_VALUE_ERORR; i++) {
		if(type_str == typeOfValueArr[i])
			return i;
	}
	return TYPE_OF_VALUE_ERORR;
}
/*============================================================*/
template <typename Tt>
Tt slow_get_page(Tt key)
{
	return key;
}
/*============================================================*/
template <typename T>
	int countCacheHits(size_t size, int N)
	{
		caches::cache_<T, T> c{size};
		T val;
		unsigned int hit = 0;
		for(int i = 0; i < N; ++i){
			std::cin >> val;

			hit += c.lookUpAndUpdate(val, slow_get_page<T>);
		}
		return hit;
	}
/*============================================================*/
int main() {
	size_t size;
	int N;
	std::cin >> size;
	std::cin >> N; 
	// int type_of_value = getTypeOfValue(type_of_value_str);
	// assert(type_of_value != TYPE_OF_VALUE_ERORR);

	unsigned int hit = 0;
	hit = countCacheHits<int>(size, N);
	
	std::cout << "Hits in cache: " << hit << std::endl;
	return 0;
}
