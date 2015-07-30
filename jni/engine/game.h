/*
 * game.h
 *
 *  Created on: 2015年7月30日
 *      Author: Ryan
 */

#ifndef GAME_H_
#define GAME_H_

#include <base.h>
#include <engine.h>

BOOL logic_init(Engine *engine);
BOOL logic_run(Engine *engine);
BOOL logic_end(Engine *engine);
void renderer_init(Engine *engine);
void renderer_end(Engine *engine);
void renderer_draw(Engine *engine, Graphic *g);



#endif /* GAME_H_ */
