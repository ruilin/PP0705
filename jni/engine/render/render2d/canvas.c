/*
 * canvas.c
 *
 *  Created on: 2013-9-21
 *      Author: ryan
 */

#include "canvas.h"

PRIVATE const GLfloat PI = 3.1415927f;
PRIVATE inline float GET_ABS(float x) {return x>0?x:-x;}

PUBLIC BOOL mTextureStateEnabled;
#define GL_ENABLE_TEXTURE()			(FALSE == mTextureStateEnabled ? ( glEnable(GL_TEXTURE_2D), mTextureStateEnabled = TRUE ) : 0)
#define GL_DISABLE_TEXTURE()		(TRUE == mTextureStateEnabled ? ( glDisable(GL_TEXTURE_2D), mTextureStateEnabled = FALSE ) : 0)

PUBLIC BOOL mScissorEnabled;
#define GL_ENABLE_SCISSOR()			(FALSE == mScissorEnabled ? ( glEnable(GL_SCISSOR_TEST), mScissorEnabled = TRUE ) : 0)
#define GL_DISABLE_SCISSOR()		(TRUE == mScissorEnabled ? ( glDisable(GL_SCISSOR_TEST), mScissorEnabled = FALSE ) : 0)

/* maps the quality (GFX_TEX_QUALITY_XXX) to specific texture parameters */
struct _tex_quality_parameters {
	GLfloat minFilter;
	GLfloat magFilter;
	GLfloat wrapModeS;
	GLfloat wrapModeT;
};

PRIVATE const struct _tex_quality_parameters _TEX_QUALITY_PARAMETERS[] = {
	/* GFX_TEX_QUALITY_NEAREST/GFX_TEX_QUALITY_LOW/GFX_TEX_QUALITY_FASTER */
	{GL_NEAREST	, GL_NEAREST, GL_REPEAT			, GL_REPEAT},
	/* GFX_TEX_QUALITY_LINEAR/GFX_TEX_QUALITY_HIGH */
	{GL_LINEAR	, GL_LINEAR , GL_CLAMP_TO_EDGE	, GL_CLAMP_TO_EDGE}
};

PRIVATE GLuint LastTexId;


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	GLint error;
    for (error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] =
	"attribute vec4 a_color;\n"
	"varying vec4 v_color;\n"

    "attribute vec4 a_position;\n"
	"attribute vec2 a_coord;\n"
	"varying vec2 v_coord;\n"
    "void main() {\n"
	"  v_color = a_color;\n"
//    "  gl_Position = a_position;\n"
	"  gl_Position = vec4(a_position.xy,0,1);\n"
	"  v_coord = a_coord;"
    "}\n";

static const char gFragmentShader[] =
	"varying vec4 v_color;\n"
    "precision mediump float;\n"
	"varying vec2 v_coord;\n"
	"uniform sampler2D s_texture;\n"
    "void main() {\n"
    "  gl_FragColor = v_color * texture2D(s_texture, v_coord);\n"
//	"  gl_FragColor = texture2D(s_texture, v_coord);\n"
//	"  gl_FragColor = v_color;\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
GLuint gvColorHandle;
GLuint gvCoordHandle;

float vertercies[] =  { -1, -1,
						  1, -1,
						 -1,  1,
						 -1,  1,
						  1,  1,
						  1, -1
						};

GLfloat vcolor[] = { 0, 1, 0, 1,    //第一个点的颜色，绿色
                     1, 0, 0, 1,  	//第二个点的颜色, 红色
                     0, 0, 1, 1,	//第三个点的颜色， 蓝色
                     0, 1, 1, 1,
                     0, 0, 1, 1,
                     0, 0, 1, 1,
					};
//为两个三角形指定索引数据
GLubyte indices[] = {
						0,1,2,  //第一个三角形索引
						2,3,1 	//第二个三角形索引
					};

typedef struct {
	float Position[2];
	float Color[4];
	float TexCoord[2];
} Vertex;
#define POSX	0.0
#define POSY	0.0
#define POSW	189
#define POSH	244
#define LT		1.0
#define VER_COUNT		4
PRIVATE Vertex texVerData[VER_COUNT] =
{
	{{POSX,			POSY},			{1,1,1,1},{0,LT}},
	{{POSX + POSW,	POSY},			{1,1,1,1},{LT,LT}},
	{{POSX,			POSY + POSH},	{1,1,1,1},{0,0}},
	{{POSX + POSW,	POSY + POSH},	{1,1,1,1},{LT,0}},
};

BOOL setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    unsigned char *gVertexShader = jni_lib_readFromAssets("shader/tex2d.vsh", NULL);
    unsigned char *gFragmentShader = jni_lib_readFromAssets("shader/tex2d.fsh", NULL);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    FREE(gVertexShader);
    FREE(gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return FALSE;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "a_position");
    gvColorHandle = glGetAttribLocation(gProgram, "a_color");
    gvCoordHandle = glGetAttribLocation(gProgram, "a_coord");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"a_position\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    /* 开启 alpha blend：支持贴图背景透明 */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*
     * glBlendFunc(GL_ONE, GL_ONE);		// 即源与目标颜色的RGBA分别相加，效果类似PS的正片叠底
     * glBlendFunc(GL_ONE, GL_ZERO); 	// 即只取源颜色，这也是默认值
     */

    /*-- VBO --*/
    /*
    GLuint vbo[2];
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(gvPositionHandle);
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vcolor), vcolor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(gvColorHandle);
    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
	*/
    /* 屏幕左边转 Opengl 世界坐标 */
    int i;
    for (i = 0; i < VER_COUNT; i++) {
    	texVerData[i].Position[0] = texVerData[i].Position[0] * 2 / w - 1.0f;
    	texVerData[i].Position[1] = texVerData[i].Position[1] * 2 / h - 1.0f;
    }
    GLuint vbo[1];
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texVerData), texVerData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(gvPositionHandle);
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid *)0);

    glEnableVertexAttribArray(gvColorHandle);
    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid *)(sizeof(float) * 2));

    glEnableVertexAttribArray(gvCoordHandle);
    glVertexAttribPointer(gvCoordHandle, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid *)(sizeof(float) * 6));
    /*glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid *)(sizeof(float) * 3));*/

    /* 指定索引数据，才可以用 glDrawElements() 绘制；使用 glDrawArrays() 则不需要 */
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	GLubyte indices[] = {
							0,1,2,  //第一个三角形索引
							2,3,1 	//第二个三角形索引
						};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return TRUE;
}

/* 绑定纹理 */
PUBLIC BOOL canvas_bind_texture(GLuint texId) {
	if (0 == LastTexId || texId != LastTexId) {
		glBindTexture(GL_TEXTURE_2D, texId);
		LastTexId = texId;
		return TRUE;
	}
	return FALSE;
}

/*
 * rescyle the texture
 */
PRIVATE void canvas_recyleTexture(void *img) {
	if (NULL == img) return;
	unsigned int texId;
	Texture *tex = (Texture *)img;
	tex->isBinded = FALSE;
	glDeleteTextures(1, &texId);
	return;
}

PRIVATE GLuint canvas_bindTexture(Graphic *g, Texture *tex) {
	if (NULL == g || NULL == tex) {
		LOGE("canvas_genTexture() NULL == g || NULL == tex");
		return -1;
	}
	if (0 == LastTexId || tex->texId != LastTexId) {
		glDeleteTextures(1, &LastTexId);
	} else {
		goto _exit;
	}
	if (TRUE == tex->isBinded) {
		goto _exit;
	}
	glGenTextures(1, &(tex->texId));
	glBindTexture(GL_TEXTURE_2D, tex->texId);
	tex->isBinded = TRUE;
	LastTexId = tex->texId;

	switch (tex->bytesPerPixel) {
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	case 2:
		/* rgb_565 not tested */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->widthPOT, tex->heightPOT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, (GLvoid *)(tex->pixels));
		break;
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex->widthPOT, tex->heightPOT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	}
	/* 设置纹理参数 */
	/* 设定缩小和放大过滤模式 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _TEX_QUALITY_PARAMETERS[tex->quality].minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _TEX_QUALITY_PARAMETERS[tex->quality].magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _TEX_QUALITY_PARAMETERS[tex->quality].wrapModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _TEX_QUALITY_PARAMETERS[tex->quality].wrapModeT);
	image_setCallBackFun(tex, canvas_recyleTexture);
_exit:
	return tex->texId;
}

/**
    * Calculates the transform from screen coordinate
    * system to world coordinate system coordinates
    * for a specific point, given a camera position.
    *
    * @param touch Vec2 point of screen touch, the
      actual position on physical screen (ej: 160, 240)
    * @param cam camera object with x,y,z of the
      camera and screenWidth and screenHeight of
      the device.
    * @return position in WCS.
    */
//PUBLIC void GetWorldCoords(float screenX, float screenY, float screenW, float screenH, float *worldX, float *worldY) {
//   // Initialize auxiliary variables.
////   Vec2 worldPos = new Vec2();
//
//   // SCREEN height & width (ej: 320 x 480)
////   float screenW = cam.GetScreenWidth();
////   float screenH = cam.GetScreenHeight();
//
//   // Auxiliary matrix and vectors
//   // to deal with ogl.
////   float[] invertedMatrix, transformMatrix,
////	   normalizedInPoint, outPoint;
////   invertedMatrix = new float[16];
////   transformMatrix = new float[16];
////   normalizedInPoint = new float[4];
////   outPoint = new float[4];
//   float invertedMatrix[16];
//   float transformMatrix[16];
//   float normalizedInPoint[4];
//   float outPoint[4];
//
//   // Invert y coordinate, as android uses
//   // top-left, and ogl bottom-left.
//   int oglTouchY = (int) (screenH - screenY);
//
//   /* Transform the screen point to clip
//   space in ogl (-1,1) */
//   normalizedInPoint[0] =
//	(float) ((screenX) * 2.0f / screenW - 1.0);
//   normalizedInPoint[1] =
//	(float) ((oglTouchY) * 2.0f / screenH - 1.0);
//   normalizedInPoint[2] = - 1.0f;
//   normalizedInPoint[3] = 1.0f;
//
//   /* Obtain the transform matrix and
//   then the inverse. */
////   Print("Proj", getCurrentProjection(gl));
////   Print("Model", getCurrentModelView(gl));
//   matrixMultiplyMM(
//	   transformMatrix, 0,
//	   getCurrentProjection(gl), 0,
//	   getCurrentModelView(gl), 0);
//   matrixInvertM(invertedMatrix, 0,
//	   transformMatrix, 0);
//
//   /* Apply the inverse to the point
//   in clip space */
//   Matrix.multiplyMV(
//	   outPoint, 0,
//	   invertedMatrix, 0,
//	   normalizedInPoint, 0);
//
//   if (outPoint[3] == 0.0)
//   {
//	   // Avoid /0 error.
//	   Log.e("World coords", "ERROR!");
//	   return worldPos;
//   }
//
//   // Divide by the 3rd component to find
//   // out the real position.
//   *worldX = outPoint[0] / outPoint[3];
//   *worldY = outPoint[1] / outPoint[3];
//
//   return worldPos;
//}

/*******************************************************************/
#include "image.h"
PRIVATE Texture *tex;

PUBLIC void canvas_init(int screenWidth, int screenHeight,
						unsigned short canvasWidth, unsigned short canvasHeight) {
	LastTexId = 0;
	setupGraphics(screenWidth, screenHeight);
	tex = (Texture *)res_newPngPOT("cat.png", IMG_QUALITY_LINEAR);

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

	return;
}

PUBLIC void canvas_end() {
	res_releasePng(tex);
}

PUBLIC void canvas_drawBitmap(Texture *tex, int x, int y) {
    GL_ENABLE_TEXTURE();
    canvas_bindTexture(engine_get()->g, tex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);
    return;
}

PUBLIC void canvas_renderTest(Graphic *g) {
//	canvas_clear(0.0f, 0.0f, 0.0f, 0.0f);
	canvas_clear(0.5f, 0.5f, 0.5f, 1.0f);
    /*glDrawArrays(GL_TRIANGLES, 0, 6);*/
	canvas_drawBitmap(tex, 1, 1);
}


