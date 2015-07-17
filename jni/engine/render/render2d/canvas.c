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

PRIVATE unsigned int LastTexId;


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

    "attribute vec4 vPosition;\n"
	"attribute vec2 a_coord;\n"
	"varying vec2 v_coord;\n"
    "void main() {\n"
	"  v_color = a_color;\n"
    "  gl_Position = vPosition;\n"
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

Vertex texVerData[] =
{
	{{-1,-1},{1,1,1,1},{0,1}},
	{{1,-1},{1,1,1,1},{1,1}},
	{{-1,1},{1,1,1,1},{0,0}},
	{{1,1},{1,1,1,1},{1,0}}
};

BOOL setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return FALSE;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    gvColorHandle = glGetAttribLocation(gProgram, "a_color");
    gvCoordHandle = glGetAttribLocation(gProgram, "a_coord");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
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
//    GLuint vbo[2];
//    glGenBuffers(2, vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(gvPositionHandle);
//    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vcolor), vcolor, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(gvColorHandle);
//    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

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

//    GLuint vbo;
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    GLfloat vcolor[] = { 0, 1, 0, 1,    //第一个点的颜色，绿色
//                         1, 0, 0, 1,  	//第二个点的颜色, 红色
//                         0, 0, 1, 1 };  //第三个点的颜色， 蓝色
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vcolor), vcolor, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(gvColorHandle);
//    glVertexAttribPointer(gvColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);


//    GLuint vertexVBO;
//    glGenBuffers(1, &vertexVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
//	float vertercies[] =
//	          {-1, -1,
//	        	1, -1,
//	           -1,  1,
//	            1,  1};
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(gvPositionHandle);
//	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

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

/*******************************************************************/
#include "image.h"
PRIVATE Texture *tex;

PUBLIC void canvas_init(int screenWidth, int screenHeight,
						unsigned short canvasWidth, unsigned short canvasHeight) {
	LastTexId = 0;
	setupGraphics(screenWidth, screenHeight);
	tex = (Texture *)res_newPngPOT("cat.png", IMG_QUALITY_LINEAR);
//	GL_ENABLE_TEXTURE();
//	GLuint textureId;
//	glGenTextures(1, &textureId);
//
//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
//
//	switch (tex->bytesPerPixel) {
//	case 4:
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
//		break;
//	case 2:
//		/* rgb_565 not tested */
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->widthPOT, tex->heightPOT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, (GLvoid *)(tex->pixels));
//		break;
//	case 1:
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex->widthPOT, tex->heightPOT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
//		break;
//	default:
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
//		break;
//	}
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    GL_ENABLE_TEXTURE();
    GLuint texId = graphic_genTexture(engine_get()->g, tex);

//    res_releasePng(tex);
	return;
}

PUBLIC void canvas_end() {
	res_releasePng(tex);
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };

PUBLIC void canvas_renderTest(Graphic *g) {
//    static float grey;
//    grey += 0.01f;
//    if (grey > 1.0f) {
//        grey = 0.0f;
//    }
//    glClearColor(grey, grey, grey, 1.0f);
//    checkGlError("glClearColor");
//    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    checkGlError("glClear");

//    glUseProgram(gProgram);
//    checkGlError("glUseProgram");

//    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
//    checkGlError("glVertexAttribPointer");
//    glEnableVertexAttribArray(gvPositionHandle);
//    checkGlError("glEnableVertexAttribArray");
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    checkGlError("glDrawArrays");

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
}


