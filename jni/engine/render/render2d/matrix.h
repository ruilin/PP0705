/*
 * matrix.h
 *
 *  Created on: 2015年7月20日
 *      Author: ryan
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <base.h>

void matrixScaleM(float *m, float x, float y, float z);
void matrixTranslateM(float *m, float x, float y, float z);
void matrixRotateM(float *m, float a, float x, float y, float z);
void matrixLookAtM(float *m,
                float eyeX, float eyeY, float eyeZ,
                float cenX, float cenY, float cenZ,
                float  upX, float  upY, float  upZ);
void matrixFrustumM(float *m, float left, float right, float bottom, float top, float near, float far);
void matrixOrthoM(float m[], int mOffset,
									float left, float right, float bottom, float top,
									float near, float far);
void matrixTransposeM(float mTrans[], int mTransOffset, float m[], int mOffset);
void matrixTranslateM2(float m[], int mOffset, float x, float y, float z);

#endif /* MATRIX_H_ */
