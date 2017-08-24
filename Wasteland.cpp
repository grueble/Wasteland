#include "SceneManager.hpp"

#define PROGRAM_NAME "Wasteland"

static const int SCREEN_WIDTH = 512;
static const int SCREEN_HEIGHT = 512;

SDL_Window* window = NULL;
SDL_GLContext glContext;
SceneNode* root = NULL;
// std::vector<ActorNode*> actors;

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
void handleKeyDown(const SDL_Event& e);
void close();

#undef main 
int main(int argc, char* argv[])
{
	if (init()) {
	  run();
   }
	close();
	return 0;
}

bool init()
{
	bool success = true;

   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      success = false;
   }
   else {
      // set texture filtering to linear
      if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
         printf( "Warning: Linear texture filtering not enabled!" );
      }

      /* SDL_GL_CONTEXT_PROFILE_MASK determines the type of context created, 
       * while both SDL_GL_CONTEXT_MAJOR_VERSION and SDL_GL_CONTEXT_MINOR_VERSION 
       * determine which version. All three attributes must be set prior to 
       * creating the first window, and in general you can't change the value 
       * of SDL_GL_CONTEXT_PROFILE_MASK without first destroying all windows 
       * created with the previous setting. */
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                          SDL_GL_CONTEXT_PROFILE_CORE);
      
      /* Turn on double buffering with a 24bit Z buffer.
       * You may need to change this to 16 or 32 for your system
       * -> defaults to double buffering on */
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

      // create window
      window = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
      if (window == NULL) {
         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
         success = false;
      }
      else {
			// Create our opengl context and attach it to our window
      	glContext = SDL_GL_CreateContext(window);

         if (glContext == NULL) {
            printf("OpenGL Context could not be created! SDL_Error %s\n", SDL_GetError());
            success = false;
         }
         else {
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
      	   if (gl3wInit()) {
      	      fprintf(stderr, "failed to initialize OpenGL\n");
      	      // return -1;
      	      success = false;
      	   }
      	   if (!gl3wIsSupported(3, 3)) {
      	      fprintf(stderr, "OpenGL 3.3 not supported\n");
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

   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glClearDepth(1.0); // this is default value (=1)

   /* create a Vertex Array Object and set it as current */
   GLuint vao; // create Vertex Array Object handle
   glGenVertexArrays(1, &vao); // allocate + assign a VAO to our handle
   glBindVertexArray(vao); // bind our VAO as the currently used obj.

   // resource initialization (vao / vbo!, shaders+, "filling", etc.)

   root = loadScene();
   // actors.push_back(loadPlayer());

   dsec t(0.0);
   dsec dt(1.0/60.0);

   auto current_time = std::chrono::high_resolution_clock::now();
   dsec accumulator(0.0); 

   glm::mat4 view;

   // main loop
   while (!quit)
   {
      // input handling
   	while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT :
				{
					quit = true;
				}
            case SDL_MOUSEBUTTONDOWN:
            {
               if (e.button.button == SDL_BUTTON_LEFT) {
                  // handleLmbDown(e);
               }
               else if (e.button.button == SDL_BUTTON_RIGHT) {
                  // handleRmbDown(e);
               }
            }
            case SDL_KEYDOWN :
            {
               handleKeyDown(e);
            }
         }
		}

      auto new_time = std::chrono::high_resolution_clock::now();
      dsec frame_time = new_time - current_time;
      current_time = new_time;
      accumulator += frame_time;

      while (accumulator >= dt)
      {
         // std::vector<ActorNode*>::iterator it;
         // for (it = actors.begin(); it != actors.end(); ++it)
         // {
         //    (*it)->integrate(t, dt);
         // }
         accumulator -= dt;
         t += dt;
      }

      // doPhysics
      // this should handle phys (i.e. movement, collision det. and res., etc.) for every obj

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      root->render(view); // may want to pass view matrix as arg to this
      SDL_GL_SwapWindow(window); // swap buffers; finalized render frame
   }

   glDeleteVertexArrays(1, &vao); // this gets called b4 glDeleteBuffers... problem?
}

void handleKeyDown(const SDL_Event& e)
{

}

// does a segfault invalidate my close() ? might need to call this earlier / more / better
void close()
{


	// Delete our OpengL context
	SDL_GL_DeleteContext(glContext);

	// Destroy our window
	SDL_DestroyWindow(window);

	// Shutdown SDL 2
	SDL_Quit();
}
