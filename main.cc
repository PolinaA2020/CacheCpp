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

	// HOW I WANT IT TO BE, BUT...
	/*switch(type_of_value) {
	case TYPE_OF_VALUE_INT: {
		cacheLookUpAndUpdate<int, int(*)(int)>.countCacheHits(size, slow_get_page_int);
		break;
	}
	case TYPE_OF_VALUE_FLOAT: {
		cacheLookUpAndUpdate<float, float(*)(float)>.countCacheHits(size, slow_get_page_float);
		break;
	}
	case TYPE_OF_VALUE_STRING: {
		cacheLookUpAndUpdate<std::string, std::string(*)(std::string)>.countCacheHits(size, slow_get_page_string);
		break;
	}
	}*/
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
	/*class {
	public:
		T type_get_func(T key) {
			return key;
		}
	} func_type;
	*/
	/*switch(type_of_value) {
	case TYPE_OF_VALUE_INT: {
		break;
	}
	case TYPE_OF_VALUE_FLOAT: {
		using T = float;
		break;
	}
	case TYPE_OF_VALUE_STRING: {
		using T = std::string;
		break;
	}
	}*/
	/*using func_type =  T (*)(T);
	func_type type_get_func;
	switch(type_of_value) {
	case TYPE_OF_VALUE_INT: {
		using T = int;
		using func_type =  T (*)(T);
		type_get_func = &slow_get_page_int;
		break;
	}
	case TYPE_OF_VALUE_FLOAT: {
		using T = float;
		using func_type =  T (*)(T);
		type_get_func = &slow_get_page_float;
		break;
	}
	case TYPE_OF_VALUE_STRING: {
		using T = std::string;
		using func_type =  T (*)(T);
		type_get_func = &slow_get_page_string;
		break;
	}
	}*/
	//using T = int;
	
	return 0;
}
