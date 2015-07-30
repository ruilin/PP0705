/*
 * BaseCpp.h
 *
 *  Created on: 2015年7月30日
 *      Author: Ryan
 */

#ifndef BASECPP_H_
#define BASECPP_H_


extern "C" {
	#include <base.h>
	#include <engine.h>
	#include <game.h>
};

void * operator new(unsigned int size, const char *file, int line) {
    void * p = MALLOC(size);
    return p;
}
#define new new(__FILE__, __LINE__)

void operator delete(void * p) {
    FREE(p);
}

void operator delete[] (void * p) {
    FREE(p);
}

#endif /* BASECPP_H_ */
