/*
 * Sprite.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: Ryan
 */

#include <game/Sprite.h>

Sprite::Sprite(unsigned short sprId, void *own) {
	unsigned short dataId; char path[32];
	resId = sprId;
	sprintf(path, "sprite/%u.png", sprId);
	resImg = res_newPngPOT(path, IMG_QUALITY_LINEAR);
	sprintf(path, "sprite/%u.sprite", sprId);
	data = sprite_data_create(path, resImg->widthPOT, resImg->heightPOT);
	owner = own;
	height = 0;
	isMainActor = FALSE;
	frameCount = 0;
	curActionId = 0;
	curFrame = 0;
	nextFrameTime = 0;

	locX = nextX = 15;
	locY = nextY = 12;
	gridOffsetX = gridOffsetY = 0;
	drawX = drawY = 0;
	isInScreen = TRUE;
	name = "name";
	drawNameX = drawNameY = 0;
	ifShowName = FALSE;
	isEntity = TRUE;
	ifStop = FALSE;
	isBackOfBuilding = FALSE;
	isUnderShadow = FALSE;
	rect_init(&rect);

	/* 获取动画身高 */
	unsigned int tileCount = getTileCount();
	if (1 <= tileCount) {
		SpriteData_Frame_Tile *frameTile;
		unsigned short frameTileCount;
		getFrameTile(0, &frameTile, &frameTileCount);
		SpriteData_Tile *spriteTile = getTile(&frameTile[0]);
		height = spriteTile->tileHeight;
	}
}

Sprite::~Sprite() {
	sprite_data_destroy(data);
	res_releasePng(resImg);
}

SpriteData_Action *Sprite::getCurAction() {
	return &(data->array_action[curActionId]);
}

unsigned int Sprite::getTileCount() {
	SpriteData_Action *action = getCurAction();
	SpriteData_Action_Frame actionFrame = action->array_actionFrame[curFrame];
	SpriteData_Frame frame = data->array_frame[actionFrame.frameIndex];
	return frame.frameTileCount;
}

void Sprite::getFrameTile(unsigned short index,
								SpriteData_Frame_Tile **frameTile, unsigned short *count) {
	SpriteData_Action *action = getCurAction();
	SpriteData_Action_Frame actionFrame = action->array_actionFrame[curFrame];
	SpriteData_Frame frame = data->array_frame[actionFrame.frameIndex];
	*frameTile = &(frame.frameTiles[index]);
	*count = frame.frameTileCount;
	return;
}

SpriteData_Tile *Sprite::getTile(SpriteData_Frame_Tile *frameTile) {
	if (NULL == frameTile) LOGE("sprite_getTile() ERROR: frameTile == NULL");
	return &(data->array_spriteTile[frameTile->tileIndex]);
}

void drawTile(Texture *tex, Graphic *g,
						short drawX, short drawY,
						const SpriteData_Tile *tile, SPRITE_TRANS trans) {
	switch (trans) {
		case TRANS_MIRROR:
		default:
			canvas_drawBitmap(tex, g, drawX, drawY);
			break;
	}
	return;
}

bool Sprite::drawFrame(Graphic *g, short x, short y,
								float red, float green, float blue, float alpha) {
    unsigned int tileCount = getTileCount();
    if (1 <= tileCount) {
    	SpriteData_Frame_Tile *frameTile;
    	unsigned short frameTileCount;
    	getFrameTile(0, &frameTile, &frameTileCount);
    	int i;
    	for (i = 0; i < frameTileCount; i++) {
			SpriteData_Tile *spriteTile = getTile(&frameTile[i]);
			graphic_setSingleColor(g, red, green, blue, alpha);
			drawTile(resImg, g, x + frameTile[i].x,
										y + frameTile[i].y, spriteTile, frameTile[i].trans);
			graphic_setSingleColor(g, 1.0, 1.0, 1.0, 1.0);
			/* 设置矩形区域 */
			rect_set(&rect, x + frameTile[i].x, y + frameTile[i].y,
					spriteTile->tileWdith, spriteTile->tileHeight);
			/* TEST 画边框 */
//			canvas_draw_Rect(g, &sprite->rect);
    	}
    	return true;
    }
    canvas_drawBitmap(resImg, g, 10, 10);
    return false;
}


