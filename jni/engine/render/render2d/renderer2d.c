/*
 * renderer2d.c
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#include "renderer2d.h"


PUBLIC void renderer2d_init(Engine *engine) {
	canvas_init(engine->screenWidth, engine->screenHeight, engine->gameWidth, engine->gameHeight);
	renderer_init(engine);
	return;
}

PUBLIC void renderer2d_end(Engine *engine) {
	canvas_end();
	renderer_end(engine);
	return;
}

PUBLIC void renderer2d_draw(Engine *engine) {
	canvas_renderTest(engine->g);
//	if (FALSE == engine->isLoading) {
		renderer_draw(engine, engine->g);
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
	return;
}
