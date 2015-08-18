// Conventions:
// Include local files with "", external files with <>
// Include local files before external files
// g_ : global
// m_ : member
// s_ : static
// Capitalized functions are public or static/global
// unCapitalized camel is for private/protected member functions
// I'm going to see how long I can avoid the standard namespace

#include "GL_Includes.h"
#include "Util.h"
#include "Shader.h"
#include "GeomFactory.h"
#include "GeomCollection.h"
#include "Camera.h"

#include <gtx/transform.hpp>

#include <SDL.h>
#include <SDL_events.h>

#include <pywrapper.h>
#include <EcsPython.h>
#define ECS_EXPOSE(Var) Ecs_Expose_Object(&Var, xstr(Var))

// Screen dims
const int WIDTH = 600;
const int HEIGHT = 600;

// GL version
const int glMajor(3), glMinor(0);

// OpenGL context and main window
SDL_GLContext g_Context;
SDL_Window * g_Window = nullptr;

// Pointer to the shader struct (does it need to be a pointer?)
Shader g_Shader;

// This doesn't need to be global, but what happens when
// it goes out of scope? Will python know?
// You may need a custom deleter...
std::unique_ptr<GeomFactory> g_GeomFactory(new QuadFactory());

// Global geometry collection
GeomCollection g_GeomCollection;

// Global Camera (you should move this somewhere)
Camera g_Camera;

// initGL
// Sets up OpenGL context, if possible
bool InitGL() {
	//Init SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Init SDL+OpenGL Context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Create Window
	g_Window = SDL_CreateWindow("3D Test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (g_Window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create Context from Window
	g_Context = SDL_GL_CreateContext(g_Window);
	if (g_Context == NULL) {
		printf("OpenGL context was not created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	//OpenGL settings
	glClearColor(1.f, .4f, .4f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE_ARB);

	//These are legacy calls valid in OpenGL 3.0 only
#ifndef __APPLE__
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

	//For debugging
	glLineWidth(8.f);

	// Shader creation
	//g_ShaderPtr = Shader::FromFile("../Shaders/simple.vert", "../Shaders/simple.frag");
	//if (g_ShaderPtr == nullptr)
	//	return false;

	return true;
}

// Python registration stuff, it's a work in progress
// Expose the geometry factory and its modifier methods
ECS_REGISTER_CLASS(GeomFactory)
ECS_REGISTER_METHOD_VOID(GeomFactory, setTrans, float, float, float)
ECS_REGISTER_METHOD_VOID(GeomFactory, setScale, float, float, float)
ECS_REGISTER_METHOD_VOID(GeomFactory, setRot, float, float, float, float)
ECS_REGISTER_METHOD_VOID(GeomFactory, setColor, float, float, float, float)

// Expose the Geom collection and it's add method
ECS_REGISTER_CLASS(GeomCollection)
ECS_REGISTER_METHOD_VOID(GeomCollection, AddGeom, void *)

ECS_REGISTER_CLASS(Shader)
ECS_REGISTER_METHOD_VOID(Shader, SetSource_V, std::string)
ECS_REGISTER_METHOD_VOID(Shader, SetSourceFile_V, std::string)
ECS_REGISTER_METHOD_VOID(Shader, SetSource_F, std::string)
ECS_REGISTER_METHOD_VOID(Shader, SetSourceFile_F, std::string)
ECS_REGISTER_METHOD_RETURN(Shader, CompileAndLink, int)

// I can't expose constructors, I'd need
// a factory (and fuck that)
ECS_REGISTER_CLASS(Camera)
ECS_REGISTER_METHOD_VOID(Camera, Init, float, float, float, float , float, float)

// initPython
// Sets up python interpreter (work in progress)
bool InitPython() {
	Ecs_Init_GeomFactory();
	Ecs_Init_GeomFactory_setTrans();
	Ecs_Init_GeomFactory_setScale();
	Ecs_Init_GeomFactory_setRot();
	Ecs_Init_GeomFactory_setColor();

	Ecs_Init_GeomCollection();
	Ecs_Init_GeomCollection_AddGeom();

	Ecs_Init_Shader();
	Ecs_Init_Shader_SetSource_V();
	Ecs_Init_Shader_SetSourceFile_V();
	Ecs_Init_Shader_SetSource_F();
	Ecs_Init_Shader_SetSourceFile_F();
	Ecs_Init_Shader_CompileAndLink();

	Ecs_Init_Camera();
	Ecs_Init_Camera_Init();

	Ecs_Initialize();

	ECS_EXPOSE(g_GeomCollection);
	ECS_EXPOSE(g_Shader);
	ECS_EXPOSE(g_Camera);
	Ecs_Expose_Object(g_GeomFactory.get(), xstr(g_GeomFactory)); // unfortunate

	return true;
}

// InitScene
// Initialize geometry using preexisting python script
// as well as camera and other stuff
bool InitScene(){


	// Set up position handle (needed for VAO creation)
	

	// run script, call functions
	Ecs_Python_File("../Scripts/init.py");
	Ecs_Python_Cmd("initShader()");

	// bind shader
	auto sBind = g_Shader.ScopeBind();
	GeomFactory::setPosHandle(g_Shader.getHandle("a_Pos"));
	Ecs_Python_Cmd("initGeom()");

	// Makes an ortho camera
	
	Ecs_Python_Cmd("initCamera()");

	// Do some python error checking!
	return true;
}

// Init
// init everything
bool Init() {
	if (!(InitGL() && InitPython()))
		return false;

	return InitScene();
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get projection matrix
	mat4 P = g_Camera.GetMat();

	// Bind shader
	auto sb = g_Shader.ScopeBind();
	for (const auto& G : g_GeomCollection) {
		// calculate PMV matrix and upload
		mat4 PMV = P * G.MV;
		glUniformMatrix4fv(g_Shader.getHandle("u_PMV"), 1, GL_FALSE, (const GLfloat *)&PMV);

		// Upload color
		glUniform4f(g_Shader.getHandle("u_Color"), G.color[0], G.color[1], G.color[2], G.color[3]);

		// Bind VAO, draw
		glBindVertexArray(G.VAO);
		glDrawElements(GL_TRIANGLE_FAN, G.nIdx, GL_UNSIGNED_INT, NULL);
	}

	SDL_GL_SwapWindow(g_Window);
}

// tearDown
// close down resources
bool TearDown() {
	SDL_DestroyWindow(g_Window);
	g_Window = nullptr;
	SDL_Quit();

	Ecs_Finalize();

	return true;
}

int main(int argc, char * argv[]) {
	Init();

	bool quit = false;
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN)
				quit = e.key.keysym.sym == SDLK_ESCAPE;
		}
		Draw();
	}

	TearDown();
	return EXIT_SUCCESS;
}
