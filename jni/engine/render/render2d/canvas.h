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

#include "matrix.h"
#include "graphic.h"
#include "text.h"

typedef enum {
	ALIGN_RIGHT		= 0,
	ALIGN_LEFT		= 1,
	ALIGN_CENTER_HORIZONTAL	= 2,
	ALIGN_CENTER_VERTICAL	= 3,
	ALIGN_CENTER	= 4,
} CANVAS_ALIGN;

static inline void canvas_clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
    /*checkGlError("glClearColor");*/
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    /*checkGlError("glClear");*/
}

void canvas_init(int screenWidth, int screenHeight,
				unsigned short canvasWidth, unsigned short canvasHeight);
void canvas_end();
void canvas_reset(unsigned short canvasWidth, unsigned short canvasHeight);
void canvas_drawBitmap(Texture *tex, Graphic *g, int x, int y);
void canvas_drawBitmapClipScale(Texture *tex, Graphic *g, int x, int y, int w, int h,
															float ratioSX, float ratioSY, float ratioEX, float ratioEY);
void canvas_renderTest(Graphic *g);

#endif /* CANVAS_H_ */
