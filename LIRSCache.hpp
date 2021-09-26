#pragma once

#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>

enum TypeOfValue {
	TYPE_OF_VALUE_INT,
	TYPE_OF_VALUE_FLOAT,
	TYPE_OF_VALUE_STRING,
	TYPE_OF_VALUE_ERORR,
};

const std::string typeOfValueArr[] = {"int", "float", "string"};

int getTypeOfValue(const std::string type_str)
{
	for(int i = 0; i < TYPE_OF_VALUE_ERORR; i++) {
		//if(strcmp(type_str, typeOfValueArr[i]))
		if(type_str == typeOfValueArr[i])
			return i;
	}
	return TYPE_OF_VALUE_ERORR;
}