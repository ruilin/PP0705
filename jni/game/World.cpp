/*
 * World.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: Ryan
 */

#include "World.h"

Sprite *s;

BOOL logic_init(Engine *engine) {
	s = new Sprite(1000, NULL);
	return TRUE;
}

BOOL logic_run(Engine *engine) {
	s->updateFrame(engine->gametime, NULL);
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
//	canvas_renderTest(g);
	canvas_clear(0.0f, 0.0f, 0.0f, 0.0f);
	s->drawFrame(g, 100, 100, 1.0f, 1.0f, 1.0f, 1.0f);
	return;
}
