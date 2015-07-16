/*
 * canvas.h
 *
 *  Created on: 2013-9-21
 *      Author: ryan
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <base.h>
#include <engine.h>
#include <resource/image.h>
#include <data/rect.h>

#include "graphic.h"
#include "text.h"

typedef enum {
	ALIGN_RIGHT		= 0,
	ALIGN_LEFT		= 1,
	ALIGN_CENTER_HORIZONTAL	= 2,
	ALIGN_CENTER_VERTICAL	= 3,
	ALIGN_CENTER	= 4,
} CANVAS_ALIGN;


void canvas_init(int screenWidth, int screenHeight,
				unsigned short canvasWidth, unsigned short canvasHeight);
void canvas_renderTest(Graphic *g);

#endif /* CANVAS_H_ */
