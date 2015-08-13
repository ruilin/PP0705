/*
 * World.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: Ryan
 */

#include "World.h"

#define PET_W		400
#define PET_H		400

Sprite *s;
int x;
int y;

BOOL logic_init(Engine *engine) {
	s = new Sprite(1000, NULL);
	x = engine->screenWidth - PET_W;
	y = engine->screenHeight - PET_H;
	return TRUE;
}

BOOL logic_run(Engine *engine) {
	if (s->updateFrame(engine->gametime, NULL)) {
//		x -= 20;
//		y += 15;
//		Javalib::resetViewLayout(x, y, PET_W, PET_H);
	}
	return TRUE;
}
BOOL logic_end(Engine *engine) {
	delete s;
	return TRUE;
}
void renderer_init(Engine *engine) {
	return;
}
void renderer_end(Engine *engine) {
	return;
}
void renderer_draw(Engine *engine, Graphic *g) {
//	graphic_pushMatrix(g);
//	matrixOrthoM(g->matrix, 0, 0, 200, 0, 200, -1.0, 1.0);
//	matrixTranslateM(g->matrix, 1.0, 0, 0);
//	matrixScaleM(g->matrix, 2.0, 2.0, 1.0);
//	canvas_renderTest(g);
//	canvas_clear(0.0f, 0.0f, 0.0f, 0.0f);
	canvas_clear(0.5f, 0.5f, 0.5f, 1.0f);
	s->drawFrame(g, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f);
//	graphic_popMatrix(g);
	return;
}
