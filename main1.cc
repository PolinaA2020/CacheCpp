// main for LIRSCache
#include "LIRSCache.cc"
#include "LIRSCache.hpp"

int slow_get_page_int(int key)
{
	return key;
}/*============================================================*/

float slow_get_page_float(float key)
{
	return key;
}
/*============================================================*/
std::string slow_get_page_string(std::string key)
{
	return key;
}
/*============================================================*/
template <typename T, typename F>
struct cacheLookUpAndUpdate {
 
	void countCacheHits(size_t size, F type_get_func)
	{
		caches::cache_<T> c{size};
		T val;
		std::cout << "Enter number of elements\n";
		int N;
		std::cin >> N;
		std::cout << "Enter value\n";
		
		for(int i = 0; i < N; i++) {
			std::cin >> val;
			c.lookUpAndUpdate<F>(val, type_get_func); // trouble func
		}
		c.printCache();
	}
};
/*============================================================*/
int main(int argc, char** argv) {
	std::string type_of_value_str = argv[1];
	size_t size = atoi(argv[2]);
	
	int type_of_value = getTypeOfValue(type_of_value_str);
	assert(type_of_value != TYPE_OF_VALUE_ERORR);
	using T = int;
	caches::cache_<T> c{size};
	T val;
	std::cout << "Enter number of elements\n";
	int N;
	std::cin >> N;
	std::cout << "Enter value\n";
	
	for(int i = 0; i < N; i++) {
		std::cin >> val;
		c.lookUpAndUpdate<T(*)(T)>(val, slow_get_page_int); // trouble func
	}
	c.printCache();

	return 0;
}
