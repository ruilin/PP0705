/*
 * engine.h
 *
 *  Created on: 2013-9-3
 *      Author: ryan
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdlib.h>
#include <GLES/gl.h>
#include <base.h>
#include <jni.h>
#include <resource/image.h>
#include <ui/motion_event.h>
#include <ui/ui.h>
#include "debug.h"
#include "sysinfo.h"


typedef BOOL (*ENGINE_CALLBACK) (void *param);

typedef struct {
	unsigned long long gametime; /* ms */
	Debug *debug;
	JNIEnv *env;
	struct Sysinfo *sysinfo;
    int32_t state;
    int32_t screenWidth;
    int32_t screenHeight;
    unsigned short gameWidth;
    unsigned short gameHeight;
    void *data;
    void *g;
    void *linked_event;	/* linked list of motion events from java */
    /* loading state */
    BOOL isLoading;
    float loadingRatio;
    float maxRatio;
    /* view transform effect */
    ENGINE_CALLBACK transEffect_callback_end;
    void *transEffect_callback_param;
    int transEff_drawingParam;
    BOOL transEff_ifInited;
    BOOL transEff_ifDrawing;
} Engine;

Engine *engine_init(JNIEnv * env);
void engine_end();
Engine *engine_get();
void engine_resetEnv(JNIEnv * env);
void engine_scaleScreenToGame(float sx, float sy, int *gx, int *gy);
void engine_scaleGameToScreen(int gx, int gy, float *sx, float *sy);
BOOL engine_setLoading(BOOL yes, float maxRatio);
void engine_setLoadingRatio(float ratio);
void engine_startTransEffect(ENGINE_CALLBACK callback_effect_end, void *param, int drawingParam);
void engine_finishTransEffect();

#endif /* ENGINE_H_ */
