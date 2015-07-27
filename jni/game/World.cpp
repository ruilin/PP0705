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
    LOGE("new size: %u", size);
    LOGE("file: %s", file);
    void * p = malloc(size);
    return p;
}
#define new new(__FILE__, __LINE__)

void operator delete(void * p) {
    free(p);
}

void operator delete[] (void * p) {
    free(p);
}

void logic_init() {
	Sprite *w = new Sprite();
	delete w;
//	LOGE("Xxxxxxxxxxxxx  %s  %u", __FILE__, __LINE__);
	return;
}

