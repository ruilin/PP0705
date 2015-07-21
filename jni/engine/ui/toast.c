/*
 * toast.c
 *
 *  Created on: 2015年1月4日
 *      Author: ryan
 */

#include "toast.h"

typedef enum {
	TOAST_STATE_UP			= 0,
	TOAST_STATE_STAY		= 1,
	TOAST_STATE_DISAPPEAR	= 2,
	TOAST_STATE_END			= 3,
} TOAST_STATE;

typedef struct {
	Linked_Item linked_item;
	const char *str;
	short drawY;
	short frame;
	float alpha;
	TOAST_STATE state;
} Toast_Msg;

PRIVATE Toast_Msg *toast_msg_create(const char *text) {
	Toast_Msg *msg = MALLOC(sizeof(Toast_Msg));
	if (NULL == msg) {
		LOGE("toast_msg_create() Error: NULL == msg");
		return NULL;
	}
	linked_list_item_init(&msg->linked_item);
	msg->str = text;
	msg->drawY = 0;
	msg->frame = 0;
	msg->alpha = 0.0f;
	msg->state = TOAST_STATE_UP;
	return msg;
}

PRIVATE void toast_msg_destroy(void *_msg) {
	Toast_Msg *msg = (Toast_Msg *)_msg;
	if (NULL != msg) {
		FREE(msg);
	}
	return;
}

/******************************************************************************/
#define TOAST_FRAME_TIME		60

typedef struct {
	unsigned long long nextFrameTime;
	void *link_msg;
	unsigned short canvasWidth;
	unsigned short canvasWidth_half;
	unsigned short canvasHeight;
	unsigned short canvasHeight_half;
} Toast;

PRIVATE Toast toast;

PUBLIC void toast_set(const char *text) {
	Toast_Msg *msg = toast_msg_create(text);
	linked_list_addItem(toast.link_msg, &msg->linked_item, msg);
	msg->drawY = toast.canvasHeight_half + 130;
	return;
}

PUBLIC void toast_init(unsigned short canvasWidth, unsigned short canvasHeight) {
	toast.link_msg = linked_list_create(toast_msg_destroy);
	toast.nextFrameTime = 0;
	toast.canvasWidth = canvasWidth;
	toast.canvasWidth_half = canvasWidth >> 1;
	toast.canvasHeight = canvasHeight;
	toast.canvasHeight_half = canvasHeight >> 1;
	return;
}

PUBLIC void toast_end() {
	linked_list_destroy(toast.link_msg); toast.link_msg = NULL;
	return;
}

PUBLIC void toast_render(Graphic *g) {
	if (TRUE == linked_list_isEmpty(toast.link_msg))
		return;
	/* frame roll */
	if (engine_get()->gametime >= toast.nextFrameTime) {
		toast.nextFrameTime = engine_get()->gametime + TOAST_FRAME_TIME;
		void *iterator = linked_list_iterator(toast.link_msg, LINKED_LIST_TAIL);
		while (NULL != iterator) {
			Toast_Msg *msg = linked_list_iterator_prev(&iterator);
			switch (msg->state) {
				case TOAST_STATE_UP:
					if (1.0f > msg->alpha) {
						msg->alpha += 0.1f;
					}
					if (toast.canvasHeight_half + 120 <= msg->drawY) {
						msg->drawY--;
					} else {
						msg->state = TOAST_STATE_STAY;
					}
					break;
				case TOAST_STATE_STAY:
					msg->frame++;
					if (30 <= msg->frame) {
						msg->state = TOAST_STATE_DISAPPEAR;
					}
					break;
				case TOAST_STATE_DISAPPEAR:
					msg->alpha -= 0.05f;
					if (0.0001f >= msg->alpha) {
						msg->state = TOAST_STATE_END;
					}
					break;
				case TOAST_STATE_END:
					linked_list_removeItem(toast.link_msg, &msg->linked_item, TRUE);
					break;
				default:
					break;
			}
		}
	}
	/* drawing */
	graphic_setTextStyle(g, 24, TEXT_STYLE_FILL);
	graphic_setTextColor(g, 250, 10, 32, 255);
	graphic_setTextStrokeColor(g, 0, 0, 0, 255);
	void *iterator = linked_list_iterator(toast.link_msg, LINKED_LIST_TAIL);
	while (NULL != iterator) {
		Toast_Msg *msg = linked_list_iterator_prev(&iterator);
		graphic_setSingleColor(g, 1.0f, 1.0f, 1.0f, msg->alpha);
//		canvas_draw_string(msg->str, g, toast.canvasWidth_half, msg->drawY, ALIGN_CENTER);
		LOGE("%s", msg->str);
	}
	return;
}

