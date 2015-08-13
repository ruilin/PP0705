/*
 * matrix.c
 *
 *  Created on: 2015年7月20日
 *      Author: ryan
 */
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

#define PI 3.1415926f

#define normalize(x, y, z)                  \
{                                               \
        float norm = 1.0f / sqrt(x*x+y*y+z*z);  \
        x *= norm; y *= norm; z *= norm;        \
}
#define I(_i, _j) ((_j)+4*(_i))

PRIVATE void matrixSetIdentityM(float *m)
{
        memset((void*)m, 0, 16*sizeof(float));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
}

PRIVATE void matrixSetRotateM(float *m, float a, float x, float y, float z)
{
        float s, c;

        memset((void*)m, 0, 15*sizeof(float));
        m[15] = 1.0f;

        a *= PI/180.0f;
        s = sin(a);
        c = cos(a);

        if (1.0f == x && 0.0f == y && 0.0f == z) {
                m[5] = c; m[10] = c;
                m[6] = s; m[9]  = -s;
                m[0] = 1;
        } else if (0.0f == x && 1.0f == y && 0.0f == z) {
                m[0] = c; m[10] = c;
                m[8] = s; m[2]  = -s;
                m[5] = 1;
        } else if (0.0f == x && 0.0f == y && 1.0f == z) {
                m[0] = c; m[5] = c;
                m[1] = s; m[4] = -s;
                m[10] = 1;
        } else {
                normalize(x, y, z);
                float nc = 1.0f - c;
                float xy = x * y;
                float yz = y * z;
                float zx = z * x;
                float xs = x * s;
                float ys = y * s;
                float zs = z * s;
                m[ 0] = x*x*nc +  c;
                m[ 4] =  xy*nc - zs;
                m[ 8] =  zx*nc + ys;
                m[ 1] =  xy*nc + zs;
                m[ 5] = y*y*nc +  c;
                m[ 9] =  yz*nc - xs;
                m[ 2] =  zx*nc - ys;
                m[ 6] =  yz*nc + xs;
                m[10] = z*z*nc +  c;
        }
}

PRIVATE void matrixMultiplyMM(float *m, float *lhs, float *rhs)
{
		int i; float t[16];
        for (i = 0; i < 4; i++) {
                register const float rhs_i0 = rhs[I(i, 0)];
                register float ri0 = lhs[ I(0,0) ] * rhs_i0;
                register float ri1 = lhs[ I(0,1) ] * rhs_i0;
                register float ri2 = lhs[ I(0,2) ] * rhs_i0;
                register float ri3 = lhs[ I(0,3) ] * rhs_i0;
                int j;
                for (j = 1; j < 4; j++) {
                        register const float rhs_ij = rhs[ I(i,j) ];
                        ri0 += lhs[ I(j,0) ] * rhs_ij;
                        ri1 += lhs[ I(j,1) ] * rhs_ij;
                        ri2 += lhs[ I(j,2) ] * rhs_ij;
                        ri3 += lhs[ I(j,3) ] * rhs_ij;
                }
                t[ I(i,0) ] = ri0;
                t[ I(i,1) ] = ri1;
                t[ I(i,2) ] = ri2;
                t[ I(i,3) ] = ri3;
        }
        memcpy(m, t, sizeof(t));
}

PUBLIC void matrixScaleM(float *m, float x, float y, float z) {
		int i;
        for (i = 0; i < 4; i++)
        {
                m[i] *= x; m[4+i] *=y; m[8+i] *= z;
        }
}

PUBLIC void matrixTranslateM(float *m, float x, float y, float z) {
		int i;
        for (i = 0; i < 4; i++)
        {
                m[12+i] += m[i]*x + m[4+i]*y + m[8+i]*z;
        }
}

PUBLIC void matrixRotateM(float *m, float a, float x, float y, float z) {
        float rot[16], res[16];
        matrixSetRotateM(rot, a, x, y, z);
        matrixMultiplyMM(res, m, rot);
        memcpy(m, res, 16*sizeof(float));
}

PUBLIC void matrixLookAtM(float *m,
												float eyeX, float eyeY, float eyeZ,
												float cenX, float cenY, float cenZ,
												float  upX, float  upY, float  upZ) {
        float fx = cenX - eyeX;
        float fy = cenY - eyeY;
        float fz = cenZ - eyeZ;
        normalize(fx, fy, fz);
        float sx = fy * upZ - fz * upY;
        float sy = fz * upX - fx * upZ;
        float sz = fx * upY - fy * upX;
        normalize(sx, sy, sz);
        float ux = sy * fz - sz * fy;
        float uy = sz * fx - sx * fz;
        float uz = sx * fy - sy * fx;

        m[ 0] = sx;
        m[ 1] = ux;
        m[ 2] = -fx;
        m[ 3] = 0.0f;
        m[ 4] = sy;
        m[ 5] = uy;
        m[ 6] = -fy;
        m[ 7] = 0.0f;
        m[ 8] = sz;
        m[ 9] = uz;
        m[10] = -fz;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
        matrixTranslateM(m, -eyeX, -eyeY, -eyeZ);
}

PUBLIC void matrixFrustumM(float *m, float left, float right, float bottom, float top, float near, float far) {
        float r_width  = 1.0f / (right - left);
        float r_height = 1.0f / (top - bottom);
        float r_depth  = 1.0f / (near - far);
        float x = 2.0f * (near * r_width);
        float y = 2.0f * (near * r_height);
        float A = 2.0f * ((right+left) * r_width);
        float B = (top + bottom) * r_height;
        float C = (far + near) * r_depth;
        float D = 2.0f * (far * near * r_depth);

        memset((void*)m, 0, 16*sizeof(float));
        m[ 0] = x;
        m[ 5] = y;
        m[ 8] = A;
        m[ 9] = B;
        m[10] = C;
        m[14] = D;
        m[11] = -1.0f;
}

/**
 * Computes an orthographic projection matrix.
 *
 * @param m returns the result
 * @param mOffset
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
void matrixOrthoM(float m[], int mOffset,
								float left, float right, float bottom, float top,
								float near, float far) {
	if (left == right) {
//		throw new IllegalArgumentException("left == right");
	}
	if (bottom == top) {
//		throw new IllegalArgumentException("bottom == top");
	}
	if (near == far) {
//		throw new IllegalArgumentException("near == far");
	}

	const float r_width  = 1.0f / (right - left);
	const float r_height = 1.0f / (top - bottom);
	const float r_depth  = 1.0f / (far - near);
	const float x =  2.0f * (r_width);
	const float y =  2.0f * (r_height);
	const float z = -2.0f * (r_depth);
	const float tx = -(right + left) * r_width;
	const float ty = -(top + bottom) * r_height;
	const float tz = -(far + near) * r_depth;
	m[mOffset + 0] = x;
	m[mOffset + 5] = y;
	m[mOffset +10] = z;
	m[mOffset +12] = tx;
	m[mOffset +13] = ty;
	m[mOffset +14] = tz;
	m[mOffset +15] = 1.0f;
	m[mOffset + 1] = 0.0f;
	m[mOffset + 2] = 0.0f;
	m[mOffset + 3] = 0.0f;
	m[mOffset + 4] = 0.0f;
	m[mOffset + 6] = 0.0f;
	m[mOffset + 7] = 0.0f;
	m[mOffset + 8] = 0.0f;
	m[mOffset + 9] = 0.0f;
	m[mOffset + 11] = 0.0f;
}

/**
 * Transposes a 4 x 4 matrix.
 *
 * @param mTrans the array that holds the output inverted matrix
 * @param mTransOffset an offset into mInv where the inverted matrix is
 *        stored.
 * @param m the input array
 * @param mOffset an offset into m where the matrix is stored.
 */
void matrixTransposeM(float mTrans[], int mTransOffset, float m[], int mOffset) {
	int i;
	for (i = 0; i < 4; i++) {
		int mBase = i * 4 + mOffset;
		mTrans[i + mTransOffset] = m[mBase];
		mTrans[i + 4 + mTransOffset] = m[mBase + 1];
		mTrans[i + 8 + mTransOffset] = m[mBase + 2];
		mTrans[i + 12 + mTransOffset] = m[mBase + 3];
	}
}
