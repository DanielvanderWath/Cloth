#include "graphics.h"

#define FAIL( X ) { \
			cout << X << endl; \
			return false; \
		} \

using namespace std;

SDL_Window* Graphics::window;
SDL_GLContext Graphics::OpenGLContext;
GLuint Graphics::VAO;
GLuint* Graphics::VBOs;
GLuint Graphics::clothProgram;
int Graphics::VBOCount=0;

bool Graphics::init()
{
	//only call this function once
	static bool alreadyInitialised = false;

	if( alreadyInitialised )
		FAIL( "Graphics already Initialised" );

	//may as well set this now
	alreadyInitialised = true;

	//Initialise SDL and create a window
	if(SDL_Init(SDL_INIT_VIDEO))
		FAIL( "Failed to initialise SDL" );

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	window=SDL_CreateWindow("Cloth Physics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINWIDTH, WINHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN); 
	
	if( !window )
		FAIL( "SDL initialised but failed to create a window." );

	//Create an OpenGL context
	OpenGLContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		GLERR("Create Vertex Array");

	//shaders for cloth
	if(setupShaderProgram("default.vert", "default.frag"))
	{
		cout << "shaders failed to compile, exiting\n";
		return true;
	}
}

void Graphics::term()
{
	glDeleteVertexArrays(1, &VAO);

	if(VBOCount > 0)
		glDeleteBuffers(VBOCount, VBOs);

	delete[] VBOs;

	glDeleteProgram(clothProgram);
}

GLuint Graphics::createVBO()
{
	GLuint VBO, *temp;

	//generate a new buffer handle
	glGenBuffers(1, &VBO);
		GLERR("Failed to generate buffer")

	//create a new temporary array to hold the old handles and the new one
	try{
	temp = new GLuint[++VBOCount];
	}
	catch(bad_alloc& exc){
	cout << "Out of memory\n";
	return 0;
	}

	//populate the new array and point the old pointer to it
	if( VBOCount > 1 )
	{
		memcpy( temp, VBOs, VBOCount );
		
		delete VBOs;

	}

	VBOs = temp;

	VBOs[VBOCount - 1] = VBO;

	//return the handle we've just created
	return VBO;
}

bool Graphics::checkProgramLinkStatus(GLuint program)
{
	//see if it linked properly
	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
			GLERR("glGetProgramiv");

		if(status==GL_FALSE)
		{
			GLint loglength;
			char *log=NULL;	
			cout << "Program linking failed\n";
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
				GLERR("glGetProgramiv loglength");

			try{
			log=new char[loglength];
				glGetProgramInfoLog(program, loglength, NULL, log);
					GLERR("glGetProgramInfoLog");

				cout << "program info log:%s\n" << log;

				delete log;
			}
			catch(bad_alloc& exc){
			cout << "Not enough memory\n";
			}
		}
	}
	return false;
}

bool Graphics::checkShaderCompileStatus(GLuint shader)
{//check for a failed compile, and print the log
	int status;
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		GLERR("glGetShaderiv shader compile status");

	if(status==GL_FALSE)
	{
		int loglength;
		char *log=NULL;
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
			GLERR("glGetShaderiv GL_INFO_LOG_LENGTH");
		
		log= new char[loglength];
		if(log)
		{
			glGetShaderInfoLog(shader, loglength, NULL, log);
				GLERR("glGetShaderInfoLog");
			cout << "shader failed to compile: " << log;
			delete log;
		}
		else
			cout << "not enough memory to print info log";
	}	
	return true;
}

bool Graphics::loadShaderFromFile(const char* filename, GLuint *shader, GLuint shaderType)
{//load a shader from filename. TODO: rewrite this in C++
	FILE *file;
	char *source=NULL;
	int length;

	if(filename)
	{
		file=fopen(filename, "r");
		if(file)
		{
			fseek(file, 0, SEEK_END);
			length=ftell(file);
			source=(char*)malloc(sizeof(char)*(length+1));
			if(!source)
			{
				printf("not enough memory to load shader: %s\n", filename);
				fclose(file);
				return true;
			}
			rewind(file);
			fread(source, 1, length, file);
			fclose(file);
			
			//NULL terminator for the shader source
			source[length]=0;
		}
		else
		{
			printf("couldn't find %s shader file: %s\n", shaderType==GL_VERTEX_SHADER ? "vertex" : "fragment", filename);
		}
		*shader= glCreateShader(shaderType);
			GLERR("glCreateShader");
		
		glShaderSource(*shader, 1, (const GLchar**)&source, NULL);
			GLERR("glShaderSource");

		glCompileShader(*shader);
			GLERR("glCompilerShader"); 
		checkShaderCompileStatus(*shader);
		free(source);
	}		
	else
		return true;
	return false;
}

bool Graphics::createProgramWith2Shaders(GLuint *program, GLuint *vert, GLuint *frag)
{//TODO: rewrite this in C++
	//program
	*program=glCreateProgram();
		GLERR("glCreateProgram");

	glAttachShader(*program, *vert);
		GLERR("glAttachShader(vertex)");

	glAttachShader(*program, *frag);
		GLERR("glAttachShader(fragment)");

	//link and check for errors
	glLinkProgram(*program);
		GLERR("glLinkProgram");
	if(checkProgramLinkStatus(*program)) {printf("program failed to link\n"); return true;}
		
	glUseProgram(*program);
		GLERR("glUseProgram");

	return false;
}

bool Graphics::setupShaderProgram(const char *vertexShaderName, const char *fragmentShaderName)
{
	GLuint vertShader, fragShader;
	if(loadShaderFromFile("default.vert", &vertShader, GL_VERTEX_SHADER))
	{
		cout << "Failed to load default.vert\n";
		return true;
	}
	if(loadShaderFromFile("default.frag", &fragShader, GL_FRAGMENT_SHADER))
	{
		cout << "Failed to load default.frag\n";
		return true;
	}
	if(createProgramWith2Shaders(&clothProgram, &vertShader, &fragShader))
	{
		cout << "Failed to create shader program\n";
		return true;
	}

	return false;
}

bool Graphics::drawCloth(GLuint VBO, GLuint partWidth, GLuint partHeight)
{
	GLuint matrix, position;
	//pass in an identity matrix for the time being
	GLfloat tempIdent[]={	1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0,
				};

	//bind program and VBO
	glUseProgram(clothProgram);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		GLERR("program and buffer");

	//TODO: call BufferData with the new vertex data here

	//bind program
	glUseProgram(clothProgram);

	//pass in the temporary identity matrix
	matrix = glGetUniformLocation(clothProgram, "matrix");
	glUniformMatrix4fv(matrix, 1, GL_FALSE, tempIdent);
		GLERR("matrix");

	//pass in the vertices
	position = glGetAttribLocation(clothProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position);
		GLERR("vertex array position buffer");

	//TODO: calculate indices beforehand, as they'll never need to change (unless we start adding and removing particles from the cloth) and then pass them in here.

	//TODO: replace this with a DrawElements using the indices passed in above
	glDrawArrays(GL_POINTS, 0, partWidth*partHeight);
		GLERR("Draw mesh as points");

	//unbind everything we bound
	glDisableVertexAttribArray(position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	return false;
}

bool Graphics::copyVertsToVBO(GLuint VBO, unsigned int numVerts, GLfloat *verts)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		GLERR("bind buffer\n");
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVerts * 3, verts, GL_DYNAMIC_DRAW);
		GLERR("Copy vertex data to buffer\n");

	return false;
}

bool Graphics::swapBuffers()
{
	SDL_GL_SwapWindow(window);
	return false;
}

bool Graphics::clear()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLERR("clear");
}

bool Graphics::CheckGLError(const char *string, int line, const char *file)
{//checks GL errors. returns true if error other than GL_NO_ERROR is found
	switch(glGetError())
	{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM at " << string << ", " << file << ":" << line << endl;
			return true;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE at " << string << ", " << file << ":" << line << endl;
			return true;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION at " << string << ", " << file << ":" << line << endl;
			return true;
		case GL_STACK_OVERFLOW:
			cout << "GL_STACK_OVERFLOW at " << string << ", " << file << ":" << line << endl;
			return true;
		case GL_STACK_UNDERFLOW:
			cout << "GL_STACK_UNDERFLOW at " << string << ", " << file << ":" << line << endl;
			return true;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY at " << string << ", " << file << ":" << line << endl;
			return true;

	}
return false;
}


