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

unsigned long long Sprite::getNextFrameGameTime(unsigned long long gametime) {
	SpriteData_Action *action = getCurAction();
	SpriteData_Action_Frame actionFrame = action->array_actionFrame[curFrame];
	return (gametime + (actionFrame.delayTime));
}

/*
 * 更新帧
 * return if frame roll end
 */
bool Sprite::updateFrame(unsigned long long gametime,
											unsigned int *_frameCount) {
	SpriteData_Action *action;
	SpriteData_Action_Frame actionFrame;
	short frameTime = -1;
	BOOL ifChangedState = FALSE;
	BOOL ifUpdateLoc = FALSE;		/* 是否通知调用者帧序列结束 */
	if (0 == nextFrameTime) {
		nextFrameTime = getNextFrameGameTime(gametime);
	}
	action = getCurAction();
	if (NULL != _frameCount)
		*_frameCount = action->frameCount;
	frameCount = action->frameCount;
	if (gametime < nextFrameTime) {
		return FALSE;
	}
	if (TRUE == ifStop) {
		return FALSE;
	}
	curFrame++;
	if (curFrame == action->frameCount) {
		curFrame = 0;
	}
	nextFrameTime = (-1 == frameTime) ?
							getNextFrameGameTime(gametime) :
							(gametime + frameTime);
	return ifUpdateLoc;
}

void Sprite::drawTile(Texture *tex, Graphic *g,
						short drawX, short drawY,
						const SpriteData_Tile *tile, SPRITE_TRANS trans) {
	switch (trans) {
		case TRANS_MIRROR:
		default:
			canvas_drawBitmapClipRatio(tex, g, drawX, drawY, tile->sxRatio, tile->syRatio, tile->exRatio, tile->eyRatio);
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
		graphic_setSingleColor(g, red, green, blue, alpha);
    	for (int i = 0; i < frameTileCount; i++) {
			SpriteData_Tile *spriteTile = getTile(&frameTile[i]);
			drawTile(resImg, g, x + frameTile[i].x,
										y + frameTile[i].y, spriteTile, frameTile[i].trans);
			/* 设置矩形区域 */
			rect_set(&rect, x + frameTile[i].x, y + frameTile[i].y,
					spriteTile->tileWdith, spriteTile->tileHeight);
			/* TEST 画边框 */
//			canvas_draw_Rect(g, &sprite->rect);
    	}
		graphic_setSingleColor(g, 1.0, 1.0, 1.0, 1.0);
    	return true;
    }
//    canvas_drawBitmap(resImg, g, 10, 10);
    return false;
}


