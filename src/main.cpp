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
#include "CollisionComponent.h"
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
GraphicsFactory::u_ptr g_GraphicsFactory(nullptr);
GraphicsCollection::u_ptr g_GraphicsCollection(nullptr);

CollisionFactory::u_ptr g_CollisionFactory(nullptr);
CollisionCollection::u_ptr g_CollisionCollection(nullptr);

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

	g_CollisionFactory = CollisionFactory::u_ptr(new CollisionFactory());
	g_CollisionCollection = CollisionCollection::u_ptr(new CollisionCollection());

	return true;
}

// initPython
// Sets up python interpreter (work in progress)
bool InitPython() {
	// It's unfortunate that static functions can't be
	// virtual, but I guess it's not a big deal
	Shader::PyRegister();
	Camera::PyRegister();
	GraphicsFactory::PyRegister();
	GraphicsCollection::PyRegister();

	// I'm just doing this for the constructor
	//Python::Register_Class<G_Data,__LINE__>("G_Comp"); 

	Python::initialize();

	// Make these macros that turn var name ==> string
	Python::Expose_Object(&g_Shader, "g_Shader");
	Python::Expose_Object(&g_Camera, "g_Camera");

	Python::Expose_Object((GraphicsFactory *)g_GraphicsFactory.get(), "g_GraphicsFactory");
	Python::Expose_Object((GraphicsCollection *)g_GraphicsCollection.get(), "g_GraphicsCollection");

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
	g_Shader.CompileAndLink();

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

// Ideally these two loops are executing asynchronously
// but we'll see how that goes
void Update() {
	// Move everything
	for (auto& box : *g_CollisionCollection.get())
		box.advance();

	// Check for collisions (kind of iterative...)
	decltype(g_CollisionCollection->begin()) it1, it2;
	// For every box
	for (it1 = g_CollisionCollection->begin(); it1 != g_CollisionCollection->end(); ++it1) {
		// For every other box
		for (it2 = it1 + 1; it2 != g_CollisionCollection->end(); ++it2) {
			bool isColliding = it1->collide(*it2);
			if (isColliding) {
				// NYI
				// do something with python
			}
		}
	}

	for (auto& G : *g_GraphicsCollection.get()) {
		// Check updates from python
		// update MV, color, etc.
		// TODO calculate MV here, dont' store it
	}
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get projection matrix
	mat4 P = g_Camera.GetMat();

	// Bind shader
	auto sb = g_Shader.ScopeBind();
	for (const auto& G : *g_GraphicsCollection.get()) {
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
		Update();
		Draw();
	}

	TearDown();
	return EXIT_SUCCESS;
}
