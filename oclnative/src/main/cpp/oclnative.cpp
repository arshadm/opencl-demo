
#include "oclnative.h"

std::vector<void*>* OclNative::hello() {
    std::vector<void*>* vector = new std::vector<void*>(1);
    (*vector)[0] = (void *)100;

	return vector;
}
