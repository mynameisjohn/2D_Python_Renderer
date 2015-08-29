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
#include "GraphicsComponent.h"
#include "Camera.h"

#include <gtx/transform.hpp>

#include <SDL.h>
#include <SDL_events.h>

#include "pyliason.h"

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

// Down the road these might be singletons,
// but for right now I just want some results
GraphicsFactory::u_ptr g_GraphicsFactory;
GraphicsCollection::u_ptr g_GraphicsCollection;

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
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
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

	g_GraphicsFactory = GraphicsFactory::u_ptr(new QuadFactory());
	g_GraphicsCollection = GraphicsCollection::u_ptr(new GraphicsCollection());

	return true;
}

// initPython
// Sets up python interpreter (work in progress)
bool InitPython() {
	Python::Register_Class<Shader>("Shader");
	Python::Register_Class<Camera>("Camera");
	Python::Register_Class<GraphicsFactory>("GraphicsFactory");
	Python::Register_Class<GraphicsCollection>("GraphicsCollection");
	Python::Register_Class<G_Data>("G_Comp"); // I'm just doing this for the constructor

	// You need a macro
	std::function<void(Camera *, float, float, float, float, float, float)> camInitFn(&Camera::InitOrtho);
	Python::_add_Func<__LINE__, Camera>("InitOrtho", camInitFn, METH_VARARGS,
		"Create an Orthographic Camera");

	std::function<int(Shader *)> shaderC_L(&Shader::CompileAndLink);
	Python::_add_Func<__LINE__, Shader>("CompileAndLink", shaderC_L, METH_VARARGS,
		"Compile and Link a Shader Program");

	std::function<int(Shader *, std::string)> shader_vSrc(&Shader::SetSourceFile_V);
	Python::_add_Func<__LINE__, Shader>("InitOrtho", shader_vSrc, METH_VARARGS,
		"Set vshader src");

	std::function<int(Shader *, std::string)> shader_fSrc(&Shader::SetSourceFile_V);
	Python::_add_Func<__LINE__, Shader>("InitOrtho", shader_fSrc, METH_VARARGS,
		"set fshader src");

	// All this shit
	std::function<void(GraphicsFactory*, float, float, float)> gf_setTrans(&GraphicsFactory::setTrans);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetTrans", gf_setTrans, METH_VARARGS,
		"Set factory's translation state");

	std::function<void(GraphicsFactory*, float, float, float)> gf_setScale(&GraphicsFactory::setTrans);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetScale", gf_setScale, METH_VARARGS,
		"Set factory's scale state");

	std::function<void(GraphicsFactory*, float, float, float, float)> gf_setRot(&GraphicsFactory::setRot);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetRot", gf_setRot, METH_VARARGS,
		"Set factory's rotation state");

	std::function<void(GraphicsFactory*, float, float, float, float)> gf_setColor(&GraphicsFactory::setColor);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetColor", gf_setColor, METH_VARARGS,
		"Set factory's color state");
	
	std::function<G_Data *(GraphicsCollection *, GraphicsFactory *)> gc_Add(&GraphicsCollection::addComponent);
	Python::_add_Func<__LINE__, GraphicsCollection>("AddComponent", gc_Add, METH_VARARGS,
		"add graphics component to collection");

	Python::initialize();

	// Make these macros that turn var name ==> string
	Python::Expose_Object(&g_Shader, "g_Shader");
	Python::Expose_Object(&g_Camera, "g_Camera");

	Python::Expose_Object(g_GraphicsFactory.get(), "g_GraphicsFactory");
	Python::Expose_Object(g_GraphicsCollection.get(), "g_GraphicsCollection");

	return true;
}

// InitScene
// Initialize geometry using preexisting python script
// as well as camera and other stuff
bool InitScene(){
	// run script to generate init functions
	// In the future you can try and use pywrapper's
	// encapsulated scripts to limit things to a scope
	// but quick and d for now
	Python::RunFile("../Scripts/init.py");

	// Init shader
	Python::RunCmd("initShader()");

	// Makes an ortho camera
	Python::RunCmd("initCamera()");

	// bind shader, get position handle, init geom
	auto sBind = g_Shader.ScopeBind();
	G_Data::SetPosHandle(g_Shader.getHandle("a_Pos"));
	Python::RunCmd("initGeom()");

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
	for (const auto& G : *g_GraphicsCollection) {
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

	Python::finalize();

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
