/*
 * Javalib.h
 *
 *  Created on: 2015年8月12日
 *      Author: Ryan
 */

#ifndef JAVALIB_H_
#define JAVALIB_H_

extern "C" {
	#include <jni_lib.h>
};

class Javalib {
public:
	Javalib();
	virtual ~Javalib();
	static void resetViewLayout(int x, int y, int w, int h);
};

#endif /* JAVALIB_H_ */
