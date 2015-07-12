/*
 * maths.c
 *
 *  Created on: 2015年1月6日
 *      Author: Ryan
 */

#include "maths.h"


/*
 * 生成指定范围的随机数
 * 注：包含 min 和 max
 */
PUBLIC int maths_generateRandNum(int min, int max) {
	return (arc4random() % (max - min + 1)) + min;
}
