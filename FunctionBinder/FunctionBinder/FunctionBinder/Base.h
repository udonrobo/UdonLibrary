#pragma once

#include "FunctionBinder.h"

struct Massege_t {
	int data;
};

struct Base {
	using FunctionBinder_t = FunctionBinder<void(Massege_t)>;
};