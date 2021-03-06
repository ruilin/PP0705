/*
 * graphic.h
 *
 *  Created on: 2013-9-21
 *      Author: Administrator
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <stddef.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <base.h>
#include <resource/image.h>

typedef enum {
	TEXT_STYLE_FILL							= 0,
	TEXT_STYLE_STROKE					= 1,
	TEXT_STYLE_FILL_AND_STROKE	= 2
} TEXT_STYLE;

typedef enum {
	GRAPHIC_ROTATE_0		= 0,
	GRAPHIC_ROTATE_90		= 1,
	GRAPHIC_ROTATE_180		= 2,
	GRAPHIC_ROTATE_270		= 3
} GRAPHIC_ROTATE;

typedef enum {
	GRAPHIC_TURN_NORMAL		= 0,
	GRAPHIC_TURN_BY_X		= 1,
	GRAPHIC_TURN_BY_Y		= 2,
} GRAPHIC_TURN;

typedef struct Graphic {
	GRAPHIC_ROTATE rotate;
	GRAPHIC_TURN turnover;
	/* text set */
	int fontSize;
	int strokeWidth;
	int paintStyle;
	int textRed;				/* text color (0~255) */
	int textGreen;
	int textBlue;
	int textAlpha;
	int textRed_stroke;		/* text stroke color (0~255) */
	int textGreen_stroke;
	int textBlue_stroke;
	int textAlpha_stroke;
	float vColor[4][4];
	GLfloat matrix_save[16];
	GLfloat matrix[16];
} Graphic;

static GLfloat ORIGIN_MATRIX[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

#define graphic_pushMatrix(g) ( memcpy(g->matrix_save, g->matrix, sizeof(g->matrix)) )
#define graphic_popMatrix(g) ( memcpy(g->matrix, g->matrix_save, sizeof(g->matrix)) )
#define graphic_rotate(g, angle, x, y, z) ( matrixRotateM(g->matrix, angle, x, y, z) )

Graphic *graphic_create();
void graphic_destroy(Graphic *g);
void graphic_setSingleColor(Graphic *g, float red, float green, float blue, float alpha);
void graphic_setTextStyle(Graphic *g, int fontSize, TEXT_STYLE paintStyle);
void graphic_setTextColor(Graphic *g, int red, int green, int blue, int alpha);
void graphic_setTextStrokeColor(Graphic *g, int red, int green, int blue, int alpha);

#endif /* GRAPHIC_H_ */
