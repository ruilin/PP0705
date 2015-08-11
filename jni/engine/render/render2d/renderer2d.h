/*
 * renderer2d.h
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#ifndef RENDERER2D_H_
#define RENDERER2D_H_

#include <base.h>
#include <engine.h>

#include "graphic.h"
#include "text.h"
#include "ui/ui.h"

void renderer2d_init(Engine *engine);
void renderer2d_end(Engine *engine);
void renderer2d_draw(Engine *engine);
void renderer2d_reset(Engine *engine, unsigned short screenWidth, unsigned short screenHeight, unsigned short gameWidth, unsigned short gameHeight);

#endif /* RENDERER2D_H_ */
