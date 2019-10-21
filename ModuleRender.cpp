#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	if (err != GLEW_OK) return false;
	
	// ... check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));// Should be 2.0
	LOG("Vendor: %s", glGetString(GL_VENDOR)); 
	LOG("Renderer: %s", glGetString(GL_RENDERER)); 
	LOG("OpenGL version supported %s", glGetString(GL_VERSION)); 
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glClearDepth(1.0f); 
	glClearColor(0.f, 0.f, 0.f, 1.f); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_DEPTH_TEST); 
	glFrontFace(GL_CCW); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_TEXTURE_2D); 
	glViewport(0, 0, App->window->screen_surface.w, App->window->screen_surface.h);


	float bufferData[] = { 
		-1.0f, -1.0f, 0.0f,      
		1.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f 
	}; 
	
	glGenBuffers(1, &vertexBufferObject); 
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glEnableVertexAttribArray(0); // attribute 0            
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);            
	glVertexAttribPointer(                    
		0,                  // attribute 0                    
		3,                  // number of componentes (3 floats)                    
		GL_FLOAT,           // data type                    
		GL_FALSE,           // should be normalized?                    
		0,                  // stride                    
		(void*)0            // array buffer offset                    
	);            
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris            
	glDisableVertexAttribArray(0);   
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
}

