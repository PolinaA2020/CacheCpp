// generate normal distributed sequence
#include <iostream>
#include <cstdio>
#include <random>
#include <fstream>

int main(int argc, char** argv) {
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