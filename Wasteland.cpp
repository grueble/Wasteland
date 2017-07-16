#include <iostream>

#include <gl3w.h>
#include <SDL.h>
// #include <SDL_image.h>

#include "Scene.hpp"
#include "GameObjects.hpp" 

static const int SCREEN_WIDTH = 400;
static const int SCREEN_HEIGHT = 400;

SDL_Window* window;
SDL_GLContext glContext;

/*
 * TODO :
 * https://en.wikibooks.org/wiki/OpenGL_Programming
 * https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
 * 
 * OK, so the next step is to set up a simple 2D side-scroller... 
 * -> bounding boxes and collision detection for actors (i.e. PC & NPC)
 * -> loading from image files, scene specifications, unloading during scene transitions, etc.
 * ---> YAML docs to specify what to load, use yaml.cpp to parse to files 
 * ---> but is YAML the best choice? compare alternatives for configuration files
 * ---> ... how to format configuration information?
 */

bool init();
void run();
void close();

int main(int argc, char* argv[])
{
	if (!init()) { return -1; }

	run();

	close();

	return 0;
}

bool init()
{
	bool success = true;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      success = false;
   }
   else
   {
      // set texture filtering to linear
      // if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
      // {
      //    printf( "Warning: Linear texture filtering not enabled!" );
      // }

      // create window
      window = SDL_CreateWindow("Wasteland", 
                                 SDL_WINDOWPOS_UNDEFINED, 
                                 SDL_WINDOWPOS_UNDEFINED, 
                                 SCREEN_WIDTH, 
                                 SCREEN_HEIGHT, 
                                 SDL_WINDOW_OPENGL);

      if (window == NULL)
      {
         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
         success = false;
      }
      else 
      {

			// Create our opengl context and attach it to our window
      	glContext = SDL_GL_CreateContext(window);

         if (glContext == NULL)
         {
            printf("OpenGL Context could not be created! SDL_Error %s\n", SDL_GetError());
            success = false;
         }
         else
         {
      	   /*
      	    * SDL_GL_CONTEXT_PROFILE_MASK determines the type of context created, 
      	    * while both SDL_GL_CONTEXT_MAJOR_VERSION and SDL_GL_CONTEXT_MINOR_VERSION 
      	    * determine which version. All three attributes must be set prior to 
      	    * creating the first window, and in general you can't change the value 
      	    * of SDL_GL_CONTEXT_PROFILE_MASK without first destroying all windows 
      	    * created with the previous setting.
      	    */
      	   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
      	   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_CORE);
	   
      	   // whether the output is single or double buffered; 
      	   // -> defaults to double buffering on
      	   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
   
            // this makes our buffer swap synchronized with the monitor's vertical refresh
            SDL_GL_SetSwapInterval(1);
	   
      	   /*
      	    * initialize GLEW so that it can take care of our OpenGL calls
      	    *    #ifndef __APPLE__
             *    glewExperimental = GL_TRUE;
             *    glewInit();
             *    #endif
			    * https://github.com/skaslev/gl3w || https://github.com/skaslev/gl3w/wiki
			    */
      	   if (gl3wInit()) 
      	   {
      	      fprintf(stderr, "failed to initialize OpenGL\n");
      	      // return -1;
      	      success = false;
      	   }
      	   if (!gl3wIsSupported(4, 5)) 
      	   {
      	      fprintf(stderr, "OpenGL 4.5 not supported\n");
      	      // return -1;
      	      success = false;
      	   }
      	   printf("OpenGL %s, GLSL %s\n", 
      	   	    glGetString(GL_VERSION),
      	          glGetString(GL_SHADING_LANGUAGE_VERSION));
         }
      }
	}
   
   return success;
}

void run()
{
	// main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   // main loop
   while (!quit)
   {
   	while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT :
				{
					quit = true;
				}
			}
		}

      // Clear our buffer with a black background
      // This is the same as :
      //       SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
      //       SDL_RenderClear(&renderer);
      //
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      SDL_GL_SwapWindow(window);
   }
}

void close()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(glContext);

	// Destroy our window
	SDL_DestroyWindow(window);

	// Shutdown SDL 2
	SDL_Quit();
}