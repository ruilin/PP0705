/*
 * Sprite.h
 *
 *  Created on: 2015年7月26日
 *      Author: Ryan
 */

#ifndef SPRITE_H_
#define SPRITE_H_

extern "C" {
	#include <base.h>
	#include <resource/sprite_data.h>
	#include <res.h>
	#include <util/data/rect.h>
	#include <graphic.h>
	#include <canvas.h>
};


typedef enum {
	SPR_UPDATE_NORMAL	= 0,
	SPR_UPDATE_RESET	= 1,
	SPR_UPDATE_END		= 2,
} SPR_UPDATE_RESULT;

class Sprite {
public:
	Sprite(unsigned short sprId, void *own);
	virtual ~Sprite();
	unsigned int getTileCount();
	bool updateFrame(unsigned long long gametime,
									unsigned int *frameCount);
	bool drawFrame(Graphic *g, short x, short y,
									float red, float green, float blue, float alpha);
private:
	unsigned long long nextFrameTime;
	unsigned short resId;
	void *owner;
	BOOL isMainActor;
	SpriteData *data;
	unsigned int frameCount;
	unsigned short curActionId;
	unsigned short curFrame;
	unsigned short height;
	Texture *resImg;
	short locX;
	short locY;
	short nextX;
	short nextY;
	short gridOffsetX;
	short gridOffsetY;
	short drawX;
	short drawY;
	Rect rect;
	BOOL isInScreen;	/* 是否在屏幕范围内 */
	/* 名称 */
	const char *name;
	short drawNameX;
	short drawNameY;
	BOOL ifShowName;
	BOOL isEntity;		/* 是否为实体(可产生阻挡和碰撞) */
	BOOL ifStop;
	BOOL isBackOfBuilding;
	BOOL isUnderShadow; /* 在阴影下 */

	SpriteData_Action *getCurAction();
	void getFrameTile(unsigned short index,
									SpriteData_Frame_Tile **frameTile,
									unsigned short *count);
	SpriteData_Tile *getTile(SpriteData_Frame_Tile *frameTile);
	unsigned long long getNextFrameGameTime(unsigned long long gametime);
	void drawTile(Texture *tex, Graphic *g,
							short drawX, short drawY,
							const SpriteData_Tile *tile, SPRITE_TRANS trans);
};

#endif /* SPRITE_H_ */
