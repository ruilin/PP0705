/*
 * engine.c
 *
 *  Created on: 2013-9-3
 *      Author: ryan
 */

#include "engine.h"

PRIVATE Engine engine;

PUBLIC Engine *engine_init(JNIEnv * env) {
	engine.gametime = 0;
	engine.env = env;
	engine.state = 0;
	engine.screenWidth = 800;
	engine.screenHeight = 480;
	engine.gameWidth = 800;
	engine.gameHeight = 480;
	engine.data = NULL;
	engine.linked_event = NULL;
	engine.g = NULL;
	engine.isLoading = FALSE;
	engine.loadingRatio = 0.0f;
	engine.maxRatio = 0.0f;
	engine.debug = debug_init();
	jni_lib_init(engine.env);
	engine.sysinfo = sysinfo_init();
	file_op_init(engine.sysinfo->externalStorageDirectoryName);
	database_init();
	sysinfo_destroy(); engine.sysinfo = NULL;
	return &engine;
}

PUBLIC void engine_end() {
//	sysinfo_destroy(); engine->sysinfo = NULL;
	jni_lib_end();
	return;
}

PUBLIC Engine *engine_get() {
	return &engine;
}

PUBLIC void engine_scaleScreenToGame(float sx, float sy, int *gx, int *gy) {
	/* scales the screen (x,y) to game (x,y) */
	*gx = engine_get()->gameWidth * (int)sx / engine_get()->screenWidth;
	*gy = engine_get()->gameHeight * (int)sy / engine_get()->screenHeight;
	return;
}

PUBLIC void engine_scaleGameToScreen(int gx, int gy, float *sx, float *sy) {
	/* scales the game (x,y) to screen (x,y) */
	*sx = (float)gx * ((float)engine_get()->screenWidth / (float)engine_get()->gameWidth);
	*sy = (float)gy * ((float)engine_get()->screenHeight/ (float)engine_get()->gameHeight);
	return;
}


PUBLIC BOOL engine_setLoading(BOOL yes, float maxRatio) {
	engine.isLoading = yes;
	engine.maxRatio = maxRatio;
	return TRUE;
}

PUBLIC void engine_setLoadingRatio(float ratio) {
	engine.loadingRatio += ratio;
	if (engine.loadingRatio > engine.maxRatio) {
		engine.loadingRatio = engine.maxRatio;
	}
	return;
}

PUBLIC void engine_startTransEffect(ENGINE_CALLBACK callback_effect_end, void *param, int drawingParam) {
	engine.transEff_ifInited = FALSE;
	engine.transEff_ifDrawing = TRUE;
	engine.transEffect_callback_end = callback_effect_end;
	engine.transEffect_callback_param = param;
	engine.transEff_drawingParam = drawingParam;
	return;
}

PUBLIC void engine_finishTransEffect() {
	engine.transEff_ifDrawing = FALSE;
	if (NULL != engine.transEffect_callback_end) {
		engine.transEffect_callback_end(engine.transEffect_callback_param);
	}
	return;
}
