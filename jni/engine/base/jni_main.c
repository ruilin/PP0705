/*
 * jni_main.c
 *
 *  Created on: 2013-8-30
 *      Author: ryan
 */


#include "jni_main.h"

#define JAVA_PACKAGE_PATH_PATFORM			"com/ryangame/pet/gl/Platform"
#define JAVA_PACKAGE_PATH_INPUTEVENT		"com/ryangame/pet/gl/InputEvent"
#define JAVA_PACKAGE_PATH_INPUTEVENT_ARRAY	"[Lcom/ryangame/pet/gl/InputEvent;"

PRIVATE BOOL is2DMode;
PRIVATE unsigned long long stime = 0;
PRIVATE unsigned short count = 0;
PRIVATE Engine *engine;
/* Return current time in milliseconds */

PRIVATE jobject _updateToJava(JNIEnv * env, jobject platformObj) {
    jclass objectClass = (*env)->FindClass(env, JAVA_PACKAGE_PATH_PATFORM);
    jfieldID state = (*env)->GetFieldID(env, objectClass,"state","I");
//    jfieldID isInited = (*env)->GetFieldID(env, objectClass,"isInited","Z");
    (*env)->SetIntField(env, platformObj, state, engine->state);
//    (*env)->SetBooleanField(env, platformObj, state, 1);
	return platformObj;
}

PRIVATE void _updateToNativeC(JNIEnv *env, jobject platformObj) {
    jclass cls_Platform = (*env)->FindClass(env, JAVA_PACKAGE_PATH_PATFORM);
    jclass cls_InputEvent = (*env)->FindClass(env, JAVA_PACKAGE_PATH_INPUTEVENT);

    jfieldID fid = (*env)->GetFieldID(env, cls_Platform, "state", "I");
    engine->state = (*env)->GetIntField(env, platformObj, fid);

    fid = (*env)->GetFieldID(env, cls_Platform, "inputs", JAVA_PACKAGE_PATH_INPUTEVENT_ARRAY);
    jobjectArray obj_Array_InputEvent = (*env)->GetObjectField(env, platformObj, fid);

    /* 取InputEvent中的int[][] pointers对应的jfieldID */
    fid = (*env)->GetFieldID(env, cls_InputEvent, "pointers", "[[I");

    if (NULL != obj_Array_InputEvent) {
    	jsize eventSize = (*env)->GetArrayLength(env, obj_Array_InputEvent);
		if (0 < eventSize) {
			int i, j;
			for (i = 0; i < eventSize; i++) {
				jobject obj_InputEvent = (*env)->GetObjectArrayElement(env, obj_Array_InputEvent, i);
				if (NULL == obj_InputEvent) {
					LOGE("_main_updateToNativeC() error: NULL == obj_InputEvent; i == %d, eventSize = %d", i, eventSize);
					continue;
				}
				jobjectArray obj_Array_Pointers = (*env)->GetObjectField(env, obj_InputEvent, fid);
				if (NULL == obj_Array_Pointers) {
					LOGE("_main_updateToNativeC() error: NULL == obj_Array_Pointers");
					continue;
				}
				jsize pointerSize = (*env)->GetArrayLength(env, obj_Array_Pointers);

				MotionEvent *event = motion_event_create();
				event->pointers = motion_pointer_create(pointerSize);
				event->pointerSize = pointerSize;
				for (j = 0; j < pointerSize; j++) {
					jintArray jpointer = (*env)->GetObjectArrayElement(env, obj_Array_Pointers, j);
					jint *jintarr = (*env)->GetIntArrayElements(env, jpointer, 0);
					event->pointers[j].action = jintarr[0];
					event->pointers[j].left = jintarr[1];
					event->pointers[j].y = jintarr[2];
					/* 释放int数组元素 */
					(*env)->ReleaseIntArrayElements(env, jpointer, jintarr, 0);
				}
				if (linked_list_getSize(engine->linked_event) < 3) {
					linked_list_addItem(engine->linked_event, &event->linkedItem, event);
				} else if (((MotionEvent *)linked_list_getTail(engine->linked_event))->pointers[0].action != event->pointers[0].action) {
					((MotionEvent *)linked_list_getTail(engine->linked_event))->pointers[0].action = event->pointers[0].action;
					motion_event_destroy(event);
				} else {
					motion_event_destroy(event);
				}
			}
		}
    }
}

JNIEXPORT JNICALL void Java_com_ryangame_pet_gl_GL2JNILib_create(JNIEnv * env, jobject obj, jobject platformObj) {
	is2DMode = TRUE;
	engine = engine_init(env);
	engine->state = 99;
	engine->gametime = time_util_now_ms();
	LOGI("endian: %s", (ENDIAN_BIG == endian_get() ? "big" : "little"));
	engine->linked_event = linked_list_create(motion_event_destroy);
	/* logic */
	runnable_init(engine);
    /* render */
    if (TRUE == is2DMode)
    	renderer2d_init(engine);
	return;
}

JNIEXPORT JNICALL BOOL Java_com_ryangame_pet_gl_GL2JNILib_init(JNIEnv * env, jobject obj, jobject platformObj,
																		jint screenWidth, jint screenHeight, jint viewWidth, jint viewHeight) {
	if (NULL == engine) return FALSE;
	engine_resetEnv(env);
	engine->screenWidth = screenWidth;
	engine->screenHeight = screenHeight;
	engine->gameWidth = viewWidth;
	engine->gameHeight = viewHeight;
	renderer2d_reset(engine, screenWidth, screenHeight, viewWidth, viewHeight);
    _updateToJava(env, platformObj);

    return TRUE;
}

JNIEXPORT JNICALL jobject Java_com_ryangame_pet_gl_GL2JNILib_step(JNIEnv * env, jobject obj, jobject platformObj) {
	if (NULL == env) return platformObj;
	if (NULL == engine) return platformObj;
	engine->gametime = time_util_now_ms();
	_updateToNativeC(env, platformObj);
	runnable_run(engine);

    if (TRUE == is2DMode) {
    	renderer2d_draw(engine);
    } else {
//    	renderer3d_draw();
    }
	 count++;
	 if (engine->gametime - stime >= 1000) {
		 engine->debug->fps = count;
		 stime = engine->gametime;
		 count = 0;
	 }
	 _updateToJava(env, platformObj);
	 return platformObj;
}

JNIEXPORT JNICALL void Java_com_ryangame_pet_gl_GL2JNILib_destroy(JNIEnv *env, jobject obj, jobject platformObj) {
	renderer2d_end(engine);
	runnable_end(engine);
	linked_list_destroy(engine->linked_event);
	engine_end(); engine = NULL;
	mem_dump();
	return;
}

JNIEXPORT JNICALL jobject Java_com_ryangame_pet_gl_GL2JNILib_getGlobalData(JNIEnv *env, jobject obj, jobject platformObj) {
    jclass objectClass = (*env)->FindClass(env, JAVA_PACKAGE_PATH_PATFORM);
    jfieldID state = (*env)->GetFieldID(env, objectClass,"state","I");
    (*env)->SetIntField(env, platformObj, state, engine->state);
    return platformObj;
}
