/*
 * renderer2d.c
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#include "renderer2d.h"


PUBLIC void renderer2d_init(Engine *engine) {
	text_init();
	canvas_init(engine->screenWidth, engine->screenHeight, engine->screenWidth, engine->screenHeight);
//	renderer_init(engine);
	return;
}

PUBLIC void renderer2d_end(Engine *engine) {
	canvas_end();
//	renderer_end(engine);
	text_end();
	return;
}

PUBLIC void renderer2d_draw(Engine *engine) {
	text_update();
//	if (FALSE == engine->isLoading) {
//		renderer_draw(engine, engine->g);
//		ui_render(engine->g);
//		if (TRUE == engine->transEff_ifDrawing) {
//			if (FALSE == engine->transEff_ifInited) {
//				engine->transEff_ifInited = TRUE;
//				renderer_transEff_init(engine, engine->transEff_drawingParam);
//			}
//			renderer_transEff_draw(engine, engine->g, engine->transEff_drawingParam);
//		}
//	} else {
//		renderer_drawLoading(engine, engine->g);
//	}
	canvas_renderTest(engine->g);
	return;
}
