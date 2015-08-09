/*
 * graphic.c
 *
 *  	Created on: 2013-9-21
 *      Author: ryan
 *      纹理管理
 */

#include "graphic.h"

PUBLIC Graphic *graphic_create() {
	Graphic *g = MALLOC(sizeof(Graphic));
	if (NULL == g) {
		LOGE("graphic_create() MALLOC fail!");
		goto _exit;
	}
	g->rotate = GRAPHIC_ROTATE_0;
	g->turnover = GRAPHIC_TURN_NORMAL;
	g->fontSize = 30;
	g->strokeWidth = 0;
	g->paintStyle = 0;
	g->textRed = g->textGreen = g->textBlue = 0;
	g->textAlpha = 255;
	g->textRed_stroke = g->textGreen_stroke = g->textBlue_stroke = 0;
	g->textAlpha_stroke = 255;
	memset(g->vColor, 0, sizeof(g->vColor));

	memcpy(g->matrix, ORIGIN_MATRIX, sizeof(ORIGIN_MATRIX));
	memcpy(g->matrix_save, ORIGIN_MATRIX, sizeof(ORIGIN_MATRIX));
_exit:
	return g;
}

PUBLIC void graphic_destroy(Graphic *g) {
	if (NULL == g) {
		LOGE("graphic_destroy() FREE fail!");
		goto _exit;
	}
	FREE(g);
	g = NULL;
_exit:
	return;
}

PUBLIC void graphic_setSingleColor(Graphic *g, float red, float green, float blue, float alpha) {
	g->vColor[0][0] = g->vColor[1][0] = g->vColor[2][0] = g->vColor[3][0] = red;
	g->vColor[0][1] = g->vColor[1][1] = g->vColor[2][1] = g->vColor[3][1] = green;
	g->vColor[0][2] = g->vColor[1][2] = g->vColor[2][2] = g->vColor[3][2] = blue;
	g->vColor[0][3] = g->vColor[1][3] = g->vColor[2][3] = g->vColor[3][3] = alpha;
	return;
}

/*
 * 0 ~ 255
 */
PUBLIC void graphic_setTextColor(Graphic *g, int red, int green, int blue, int alpha) {
	g->textRed = red;
	g->textGreen = green;
	g->textBlue = blue;
	g->textAlpha = alpha;
	return;
}

/*
 * 0 ~ 255
 */
PUBLIC void graphic_setTextStrokeColor(Graphic *g, int red, int green, int blue, int alpha) {
	g->textRed_stroke = red;
	g->textGreen_stroke = green;
	g->textBlue_stroke = blue;
	g->textAlpha_stroke = alpha;
	return;
}

PUBLIC void graphic_setTextStyle(Graphic *g, int fontSize, TEXT_STYLE paintStyle) {
	g->fontSize = fontSize;
	g->strokeWidth = fontSize >> 3;
	g->paintStyle = paintStyle;
	return;
}

