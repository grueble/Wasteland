#include <chrono>

#include "OpenGLGraphics.hpp"
#include "SceneManager.hpp"

#define PROGRAM_NAME "Wasteland"

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 800;

typedef std::chrono::duration<double> dsec;

SDL_Window* window = NULL;
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

struct eye_point 
{
   eye_point(vec3_t a_cam, vec3_t a_look_at, vec3_t a_up_dir) :
      camera(a_cam),
      look_at(a_look_at),
      up_dir(a_up_dir)
   {}

   vec3_t camera;
   vec3_t look_at;
   vec3_t up_dir;
};

bool init();
void run();
void handleKeyDown(const SDL_Event& e, glm::mat4& view, eye_point& eye, Entity& plyr);
void handleKeyUp(const SDL_Event& e, Entity& plyr);
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

   Renderer_t renderer;
   renderer.init();

   SceneManager scene_mngr(renderer);
   Node& world_root = scene_mngr.getWorld();

   std::unique_ptr<Entity> player = scene_mngr.getPlayer();

   dsec t(0.0);
   dsec dt(1.0/60.0);

   auto current_time = std::chrono::high_resolution_clock::now();
   dsec accumulator(0.0); 

   eye_point eye = eye_point(
      vec3_t(2.5f, 8.0f, 20.0f), // camera
      vec3_t(5.0f, 10.0f, 0.0f),  // look at
      vec3_t(0.0f, 1.0f, 0.0f)   // up dir
   );

   glm::mat4 view = glm::lookAt(
      glm::vec3(eye.camera.x, eye.camera.y, eye.camera.z),
      glm::vec3(eye.look_at.x, eye.look_at.y, eye.look_at.z),
      glm::vec3(eye.up_dir.x, eye.up_dir.y, eye.up_dir.z)
   );

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
               break;
				}
            case SDL_MOUSEBUTTONDOWN:
            {
               if (e.button.button == SDL_BUTTON_LEFT) {
                  // handleLmbDown(e);
               }
               else if (e.button.button == SDL_BUTTON_RIGHT) {
                  // handleRmbDown(e);
               }
               break;
            }
            case SDL_KEYDOWN :
            {
               handleKeyDown(e, view, eye, *player);
               break;
            }
            case SDL_KEYUP :
            {
               handleKeyUp(e, *player);
               break;
            }
         }
		}

      auto new_time = std::chrono::high_resolution_clock::now();
      dsec frame_time = new_time - current_time;
      current_time = new_time;
      accumulator += frame_time;


      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //ok, so I've narrowed the issue down to an integration issue
      // currently, a projection is not enough to move the object out of the shape
      while (accumulator >= dt)
      {
         // std::vector<ActorNode*>::iterator it;
         // for (it = actors.begin(); it != actors.end(); ++it)
         // {
         //    (*it)->integrate(t, dt);
         //    (*it)->update();
         // }
         player->update(world_root, renderer, view);
         accumulator -= dt;
         t += dt;
      }

      world_root.update(world_root, renderer, view);
      // player->update(world_root, renderer, view);
      SDL_GL_SwapWindow(window); // swap buffers; finalized render frame
   }

   renderer.close();
}

void handleKeyDown(const SDL_Event& e, glm::mat4& view, eye_point& eye, Entity& plyr)
{
   switch(e.key.keysym.sym)
   {
      case 'w' :
      {
         if (plyr.v.y <= 0.0f)
         {
            plyr.v.y = 1.0f;
         }
         if (plyr.v.y < 1.0f) 
         {
            plyr.v.y += 0.25f;
         }
         break;
      }
      case 'a' :
      {
         plyr.v.x = -0.25f;
         break;
      }
      case 'd' :
      {
         plyr.v.x = 0.25f;
         break;
      }
      case SDLK_LEFT :
      {
         // glm::vec4 camera_pt(eye.camera.x, eye.camera.y, eye.camera.z, 1.0f);
         // glm::vec4 look_at_pt(eye.look_at.x, eye.look_at.y, eye.look_at.z, 1.0f);
         // glm::mat4 xform = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 0.0f));
         // glm::vec4 new_camera = camera_pt * xform;
         // glm::vec4 new_look_at = look_at_pt * xform; 
         eye.camera.x = eye.camera.x - 0.5f;
         eye.look_at.x = eye.look_at.x - 0.5f;
         view = glm::lookAt(
            glm::vec3(eye.camera.x, eye.camera.y, eye.camera.z),
            glm::vec3(eye.look_at.x, eye.look_at.y, eye.look_at.z),
            glm::vec3(eye.up_dir.x, eye.up_dir.y, eye.up_dir.z)
         );
         break;
      }
      case SDLK_UP :
      {
         eye.camera.y = eye.camera.y + 0.5f;
         eye.look_at.y = eye.look_at.y + 0.5f;
         view = glm::lookAt(
            glm::vec3(eye.camera.x, eye.camera.y, eye.camera.z),
            glm::vec3(eye.look_at.x, eye.look_at.y, eye.look_at.z),
            glm::vec3(eye.up_dir.x, eye.up_dir.y, eye.up_dir.z)
         );
         break;
      }
      case SDLK_RIGHT :
      {
         eye.camera.x = eye.camera.x + 0.5f;
         eye.look_at.x = eye.look_at.x + 0.5f;
         view = glm::lookAt(
            glm::vec3(eye.camera.x, eye.camera.y, eye.camera.z),
            glm::vec3(eye.look_at.x, eye.look_at.y, eye.look_at.z),
            glm::vec3(eye.up_dir.x, eye.up_dir.y, eye.up_dir.z)
         );
         break;
      }
      case SDLK_DOWN :
      {
         eye.camera.y = eye.camera.y - 0.5f;
         eye.look_at.y = eye.look_at.y - 0.5f;
         view = glm::lookAt(
            glm::vec3(eye.camera.x, eye.camera.y, eye.camera.z),
            glm::vec3(eye.look_at.x, eye.look_at.y, eye.look_at.z),
            glm::vec3(eye.up_dir.x, eye.up_dir.y, eye.up_dir.z)
         );
         break;
      }
      default :
      {
         break;
      }
   }
}

void handleKeyUp(const SDL_Event& e, Entity& plyr)
{
   switch (e.key.keysym.sym)
   {
      case 'a' :
      {
         plyr.v.x = 0.0f;
         break;
      }
      case 'd' :
      {
         plyr.v.x = 0.0f;
         break;
      }
      default :
      {
         break;
      }
   }
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
