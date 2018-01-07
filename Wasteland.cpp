#include <chrono>

#include "SceneManager.hpp"

#define PROGRAM_NAME "Wasteland"

static const int SCREEN_WIDTH = 1400;
static const int SCREEN_HEIGHT = 875;

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

bool init();
void run(Renderer_t& renderer);
void handleKeyDown(const SDL_Event& e, glm::mat4& view, eye_point& eye, Entity& plyr);
void handleKeyUp(const SDL_Event& e, Entity& plyr);
void close();

#undef main 
int main(int argc, char* argv[])
{
	if (init()) {
      Renderer_t renderer;
      renderer.init();

      run(renderer);

      renderer.close();
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

void run(Renderer_t& renderer)
{
	// main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glClearDepth(1.0); // this is default value (=1)

   SceneManager scene_mngr(renderer, window, glContext);
   scene_mngr.loadScene();

   std::vector<BoundingNode>& world = scene_mngr.getWorld();
   Actor& player = scene_mngr.getPlayer();
   Camera& camera = scene_mngr.getCamera();

   dsec t(0.0);
   dsec dt(1.0/60.0);

   auto current_time = std::chrono::high_resolution_clock::now();
   dsec accumulator(0.0); 

   glm::mat4 view = glm::mat4(1.0f);

   // main loop
   while (!quit)
   {
      std::cout << "top" << std::endl;

      // input handling
   	while (SDL_PollEvent(&e))
		{
         if (e.type == SDL_QUIT)
         {
            quit = true;
         }
         else
         {
            scene_mngr.handleInput(e);
            // handleInput(e, player);
         }
		}

      // SDL_KEYDOWN fires 
      // SDL_GetKeyState : https://gamedev.stackexchange.com/questions/19571/how-can-i-process-continuously-held-keys-with-sdl

      auto new_time = std::chrono::high_resolution_clock::now();
      dsec frame_time = new_time - current_time;
      current_time = new_time;
      accumulator += frame_time;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      while (accumulator >= dt)
      {
         // std::vector<ActorNode*>::iterator it;
         // for (it = actors.begin(); it != actors.end(); ++it)
         // {
         //    (*it)->integrate(t, dt);
         //    (*it)->update();
         // }
         camera.update(world, renderer, view); // these lines basically say: keep updating the player
         player.update(world, renderer, view); // and view, while we wait for the bottom of the frame
         accumulator -= dt;
         t += dt;
      }

      // ** instead of rendering in update, preload the vertices
      // this could be a top-level bounding node
      std::vector<BoundingNode>::iterator it;
      for (it = world.begin(); it != world.end(); ++it)
      {
         it->update(world, renderer, view);
      }
      // player->update(world_root, renderer, view);
      SDL_GL_SwapWindow(window); // swap buffers; finalized render frame
   }
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
      case 's' :
      {
         // crouch animation
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
