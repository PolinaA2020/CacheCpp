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
	int countCacheHits(size_t size, std::ifstream &f)
	{
		caches::cache_<T, T> c{size};
		T val;
		//std::cout << "Enter number of elements\n";
		//int N;
		//std::cin >> N;
		//std::cout << "Enter value\n";
		unsigned int hit = 0;
		for( ; ; ){
			f >> val;
			if (f.eof())
				break;
			hit += c.lookUpAndUpdate(val, slow_get_page<T>);
			//c.template lookUpAndUpdate<F>(val, type_get_func); 
		}
		c.printCache();
		return hit;
	}
/*============================================================*/
int main(int argc, char** argv) {
	std::string type_of_value_str = argv[1];
	size_t size = atoi(argv[2]);
	std::ifstream source(argv[3]); 
	int type_of_value = getTypeOfValue(type_of_value_str);
	assert(type_of_value != TYPE_OF_VALUE_ERORR);

	unsigned int hit = 0;
	switch(type_of_value) {
	case TYPE_OF_VALUE_INT: {
		hit = countCacheHits<int>(size, source);
		break;
	}
	case TYPE_OF_VALUE_FLOAT: {
		hit = countCacheHits<float>(size, source);
		break;
	}
	case TYPE_OF_VALUE_STRING: {
		hit = countCacheHits<std::string>(size, source);
		break;
	}
	}
	std::cout << "Hits in cache: " << hit << std::endl;
	source.close();
	return 0;
}
