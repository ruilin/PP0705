/*
 * toast.h
 *
 *  Created on: 2015年1月4日
 *      Author: ryan
 */

#ifndef TOAST_H_
#define TOAST_H_

#include <base.h>
#include <engine.h>
#include <util/linked_list.h>
#include <render2d/canvas.h>
#include <render2d/graphic.h>

void toast_set(const char *text);
void toast_init(unsigned short canvasWidth, unsigned short canvasHeight);
void toast_end(void);
void toast_render(Graphic *g);

#endif /* TOAST_H_ */
