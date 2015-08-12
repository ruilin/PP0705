/*
 * hello_jni.h
 *
 *  Created on: 2013-8-30
 *      Author: Administrator
 */

#ifndef HELLO_JNI_H_
#define HELLO_JNI_H_

#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <base.h>
#include <file_op.h>

/*
	Java 		符号
	boolean 	Z
	byte 		B
	char 		C
	short 		S
	int 		I
	long 		L
	float 		F
	double 		D
	void 		V
	objects对象 	Lfully-qualified-class-name;	L类名;
	Arrays数组 	[array-type [数组类型
	methods方法 	(argument-types)return-type(参数)返回类型
 */
JNIEnv *jlib_getEnv();
jclass jlib_getCls();
void jlib_init(JNIEnv* env);
void jlib_end();
AAsset *jlib_getAAsset(const char *filename, off_t *size);
void jlib_cpy_allFileToSdcard(const char *path);
unsigned char *jlib_readFromAssets(const char *filename, unsigned int *bufferSize);
unsigned char *jlib_readPngFromAssets(const char *filename, jobject *bitmap);
BOOL jlib_loadJavaAssetBitmap(const char *filename, jobject *bitmap);
void jlib_bitmapToPixels(jobject bitmap, unsigned char **pixels);
BOOL jlib_textToBitmapPOT(const char *strToDraw, int fontsize,
							int strokeWidth, int paintStyle,
							jobject *bitmapObject,
							int *bitmapWidth, int *bitmapHeight,
							int *bitmapWidthPOT, int *bitmapHeightPOT,
							int *textPixelSize,
							int r, int g, int b, int a,
							int strokeR, int strokeG, int strokeB, int strokeA);
BOOL jlib_getSysInfo(char **stringArray, int arrayCount, long long *longArray, int longArrayCount);

#endif /* HELLO_JNI_H_ */
