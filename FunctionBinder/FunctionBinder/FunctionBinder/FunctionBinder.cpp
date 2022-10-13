#include <iostream>
#include "Reader.h"

Reader reader;

int main() {
	std::cout << "call" << std::endl;
	FunctionBinder<void(Massege_t)>::bind({ 100 });
	std::cout << "end" << std::endl;
}