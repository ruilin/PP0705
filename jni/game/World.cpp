/*
 * World.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: Ryan
 */

#include <game/World.h>

//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;

//void* operator new(size_t, LPCSTR, int);
//void *operator new (size_t size);

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

void logic_init() {
	Sprite *w = new Sprite();
//	delete w;
//	LOGE("Xxxxxxxxxxxxx  %s  %u", __FILE__, __LINE__);
	return;
}

