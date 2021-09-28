// main for LIRSCache
#include "LIRSCache.cc"
#include "LIRSCache.hpp"

/*============================================================*/
template <typename Tt>
Tt slow_get_page(Tt key)
{
	return key;
}
/*============================================================*/
template <typename T>
	int countCacheHits(size_t size)
	{
		caches::cache_<T, T> c{size};
		T val;
		std::cout << "Enter number of elements\n";
		int N;
		std::cin >> N;
		std::cout << "Enter value\n";
		
		for(int i = 0; i < N; i++) {
			std::cin >> val;
			c.lookUpAndUpdate(val, slow_get_page<T>);
			//c.template lookUpAndUpdate<F>(val, type_get_func); 
		}
		c.printCache();
		return 0;
	}
/*============================================================*/
int main(int argc, char** argv) {
	std::string type_of_value_str = argv[1];
	size_t size = atoi(argv[2]);
	
	int type_of_value = getTypeOfValue(type_of_value_str);
	assert(type_of_value != TYPE_OF_VALUE_ERORR);

	unsigned int hit = 0;
	switch(type_of_value) {
	case TYPE_OF_VALUE_INT: {
		hit = countCacheHits<int>(size);
		break;
	}
	case TYPE_OF_VALUE_FLOAT: {
		hit = countCacheHits<float>(size);
		break;
	}
	case TYPE_OF_VALUE_STRING: {
		hit = countCacheHits<std::string>(size);
		break;
	}
	}
	cout << "Hits in cache: " << hit << std::endl;
	return 0;
}
