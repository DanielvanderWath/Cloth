#if !defined(__GRAPHICS_H__)
#define __GRAPHICS_H__

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLERR(X) if(CheckGLError(X, __LINE__, __FILE__))return true;

class Graphics
{
	private:
		static SDL_Window *window;
		static SDL_GLContext OpenGLContext;
		static const unsigned int WINWIDTH=640, WINHEIGHT=480;
		static GLuint clothProgram;
		static GLuint VAO;
		static GLuint *VBOs;
		static int VBOCount;
	public:
		static bool init();
		static void term();
		static GLuint createVBO();
		static bool checkShaderCompileStatus(GLuint shader);
		static bool checkProgramLinkStatus(GLuint program);
		static bool createProgramWith2Shaders(GLuint *program, GLuint *vert, GLuint *frag);
		static bool loadShaderFromFile(const char *filename, GLuint *shader, GLuint shaderType);
		static bool setupShaderProgram(const char *vertexShaderName, const char *fragmentShaderName);
		static bool copyVertsToVBO(GLuint VBO, unsigned int numVerts, GLfloat *verts);
		static bool drawCloth(GLuint VBO, GLuint partWidth, GLuint partHeight);
		static bool clear();
		static bool swapBuffers();
		static bool CheckGLError(const char *string, int line, const char *file);
};

#endif

