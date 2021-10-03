// generate looping pattern
#include <iostream>
#include <cstdio>
#include <random>
#include <fstream>

int main(int argc, char** argv) {
	long long size = atoll(argv[1]);
	int j = 0;
    int l = 0;
    std::string str = "loop_int_";
	str += argv[1];
	str += ".dat";
	std::ofstream f;
	f.open(str);
	std::random_device rd; 
    std::mt19937 mersenne(rd());

    for(int i = 0; i < size; i++){
        if(j == l) {
            j = 0;
            l = (mersenne() + 1) % (size / 4);
        }
        f << j << " ";
        j += 1;
    }
    f << std::endl;
    f.close();
}