/*
 * jni_main.h
 *
 *  Created on: 2013-8-30
 *      Author: Administrator
 */

#ifndef JNI_MAIN_H_
#define JNI_MAIN_H_

#include <jni.h>
#include <base.h>
#include "engine.h"
#include <runnable/runnable.h>
#include <render2d/renderer2d.h>
#include <util/time_util.h>
#include <util/mem.h>
#include <util/data/endian.h>
#include <ui/ui.h>
#include <ui/motion_event.h>

JNIEXPORT JNICALL void Java_com_ryangame_pet_gl_GL2JNILib_create(JNIEnv * env, jobject obj, jobject platformObj);
JNIEXPORT JNICALL BOOL Java_com_ryangame_pet_gl_GL2JNILib_init(JNIEnv * env, jobject obj, jobject platformObj,
																													jint screenWidth, jint screenHeight,
																													jint viewWidth, jint viewHeight);
JNIEXPORT JNICALL jobject Java_com_ryangame_pet_gl_GL2JNILib_step(JNIEnv * env, jobject obj, jobject platformObj);
JNIEXPORT JNICALL void Java_com_ryangame_pet_gl_GL2JNILib_destroy(JNIEnv *env, jobject obj, jobject platformObj);
JNIEXPORT JNICALL jobject Java_com_ryangame_pet_gl_GL2JNILib_getGlobalData(JNIEnv *env, jobject obj, jobject platformObj);

#endif /* JNI_MAIN_H_ */
