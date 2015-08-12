/*
 * Javalib.cpp
 *
 *  Created on: 2015年8月12日
 *      Author: Ryan
 */

#include <Javalib.h>

Javalib::Javalib() {
}

Javalib::~Javalib() {
}

void Javalib::resetViewLayout(int x, int y, int w, int h) {
	/* call static method */
	JNIEnv *env = jlib_getEnv();
	jclass jcls = jlib_getCls();
	jmethodID mId = (env)->GetStaticMethodID(jcls, "resetViewLayout", "(IIII)V");
	(env)->CallStaticVoidMethod(jcls, mId, x, y, w, h);
	return;
}
