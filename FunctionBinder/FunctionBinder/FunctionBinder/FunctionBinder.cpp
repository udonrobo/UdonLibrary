#include <iostream>
#include "FunctionBinder.h"

#if 1

struct Sample : public FunctionBinder<void(int)>, public FunctionBinder<void(int, int)>{
	void FunctionBinder<void(int)>::callback(int arg) override {
		std::cout << arg << std::endl;
	}
	void FunctionBinder<void(int, int)>::callback(int arg1, int arg2) override {
		std::cout << arg1 << '\t' << arg2 << std::endl;
	}
};

#else

struct Sample : public FunctionBinder<void(int)> {
	void callback(int arg) override {
		std::cout << arg << std::endl;
	}
};

#endif
int main() {
	Sample samp0;
	Sample samp1;
	Sample samp2;

	FunctionBinder<void(int)>::bind(100);
	FunctionBinder<void(int, int)>::bind(10, 20);
}