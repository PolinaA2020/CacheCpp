// generate tests normal distribution for strings
#include <iostream>
#include <cstdio>
#include <random>
#include <fstream>
#include <string>

char alph[26];
int main(int argc, char** argv) {

	for(int i =0; i < 26; i++) {
		alph[i] = 65 + i;
	}

	long long size = atoll(argv[1]);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> X(0, 1000);
	std::vector<int> data(size);
	for (int &var : data)
	   var = X(gen);
	std::string str = "int_";
	str += argv[1];
	str += ".dat";
	std::ofstream f;
	f.open(str);
	for(auto it = data.begin(); it != data.end(); ++it) {
		f << *it << " ";
	}
	f << std::endl;
	f.close();
}