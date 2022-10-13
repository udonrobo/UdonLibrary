#pragma once


#include "FunctionBinder.h"

struct Massege_t {
	int data;
};

struct Base {
	using FunctionBinder_t = FunctionBinder<Massege_t>;
};

struct Reader : private Base, private FunctionBinder<void(Massege_t)> {

	void callback(Massege_t arg) override {
		std::cout << arg.data << std::endl;
	}
};