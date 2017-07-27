#include <stdlib.h>
#include <stdio.h>

#include <gl3w.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "Scene.hpp"
#include "GameObjects.hpp" 

#define PROGRAM_NAME "Wasteland"

static const int SCREEN_WIDTH = 512;
static const int SCREEN_HEIGHT = 512;

/* NOTE: **screen** coords are always of the form: 
 * -> ( [-1.0f, 1.0f], [-1.0f, 1.0f] ) */
static const GLfloat triangle_coord_data[] = {
   -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

static const GLfloat color_data[] = {
   1.0f, 0.0f, 0.0f, // red
   0.0f, 1.0f, 0.0f, // green
   0.0f, 0.0f, 1.0f, // blue
   1.0f, 1.0f, 1.0f, // white
};

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
void draw();
GLuint loadShaders(const char* vertex_fpath, const char* fragment_fpath);
void loadScene();
void run();
void close();
char* filetobuf(const char *file);

#undef main
int main(int argc, char* argv[])
{
	if (!init()) { return -1; }

	run();

   // draw();

   // SDL_Delay(2000);

	close();

	return 0;
}

bool init() //setupwindow
{
	bool success = true;

   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      success = false;
   }
   else {
      // set texture filtering to linear
      // if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
      // {
      //    printf( "Warning: Linear texture filtering not enabled!" );
      // }

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

void draw()
{
   // ***************************
   // for tmrw (7/26)... incorporate this into the main loop! and make sure it can draw a white tri b4 shader adds
   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glClearDepth(1.0); // this is default value (=1)

   /* create a Vertex Array Object and set it as current */
   GLuint vao; // create Vertex Array Object handle
   glGenVertexArrays(1, &vao); // allocate + assign a VAO to our handle
   glBindVertexArray(vao); // bind our VAO as the currently used obj.

   GLuint programID = loadShaders("wasteland.vert", "wasteland.frag");

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glUseProgram(programID); // use our shader

   /* create two Vertex Buffer Objects to give to OpenGL */ 
   GLuint vbo[2]; // create Vertex Buffer Object handle
   glGenBuffers(2, vbo); // allocate + assign two VBOs to our handle

   /* bind our 1st VBO as the active buffer for storing vertex attributes 
    * -> (coordinates in this case) */
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 
   /* copy the vertex data from our triangle data struct to the active buffer */
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_coord_data),
                triangle_coord_data, GL_STATIC_DRAW);
   /* specify that our coordinate data is going into 
    * attr. index 0, and contains 3 floats per vertex */
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(0); // enable attr. @ index 0 (triangle coordinates) for use

   /* bind our 2nd VBO as the active buffer for storing vertex attributes 
    * -> (colors in this case) */
   glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
   /* copy the vertex data from our color data struct to the active buffer */
   glBufferData(GL_ARRAY_BUFFER, sizeof(color_data),
                color_data, GL_STATIC_DRAW);
   /* specify that our color data is going into 
    * attr. index 0, and contains 3 floats per vertex */
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(1); // enable attr. @ index 1 (colors) for use

   glDrawArrays(GL_TRIANGLES, 0, 3); // draw call

   SDL_GL_SwapWindow(window); // swap buffers; finalized render frame

   glUseProgram(0);
   glDisableVertexAttribArray(0); // disable attr. @ index 0 for use 
   glDisableVertexAttribArray(1); // disable attr. @ index 1 for use
   glDeleteProgram(programID); 
   // glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind buffer 
   glDeleteBuffers(2, vbo);
   glDeleteVertexArrays(1, &vao); 
}

GLuint loadShaders(const char* vertex_fpath, const char* fragment_fpath)
{
   // create empty handles for our shaders
   GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

   // read our shaders into the appropriate buffers
   GLchar* vertex_src = filetobuf(vertex_fpath);
   GLchar* fragment_src = filetobuf(fragment_fpath);

   GLint result = GL_FALSE;
   // int log_length;

   // send the vertex shader src code to GL; compile; check for (and handle?) errors
   glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_src, 0);
   glCompileShader(vertex_shader);
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) 
   {
      printf("Vertex shader compilation failure!");
      // glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &result);
   }

   // send the fragment shader src code to GL; compile; check for (and handle?) errors
   glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_src, 0);
   glCompileShader(fragment_shader);
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) 
   {
      printf("Fragment shader compilation failure!");
      // glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &result);
   }

   // create an empty handle for our linked shader program
   GLuint shader_program;

   // attach our shaders to our program
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);

   /* bind attr. @ index 0 (coordinates) to in_Position and attr. @ index 1 (color) to in_Color
    * -> NOTE: attr. locations must be setup before calling glLinkProgram */
   glBindAttribLocation(shader_program, 0, "in_Position");
   glBindAttribLocation(shader_program, 1, "in_Color");
   glLinkProgram(shader_program); // link our program

   // check for proper linkage between the vertex and fragment shaders (and other errors)
   glGetProgramiv(shader_program, GL_LINK_STATUS, (int*)&result);
   if (result == GL_FALSE)
   {
      printf("Shader program linkage failure!");
      // glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, (int*)&log_length);
   }

   glDetachShader(shader_program, vertex_shader);
   glDetachShader(shader_program, fragment_shader);
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   free(vertex_src);
   free(fragment_src);

   return shader_program;   
}

void loadScene()
{

}

void run()
{
	// main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   // resource initialization (vao / vbo!, shaders+, "filling", etc.)

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
            // case :
            // handleKeyDown
            // handleButtonDown
            // these two functions should update the stored data about the player actor

            // doPhysics
            // this should handle phys (i.e. movement, collision det. and res., etc.) for every obj

            // draw 
            // redraw the scene every loop
			}
		}
      // glClearColor(0.0, 0.0, 0.0, 1.0);
      // glClearDepth(1.0); // this is default value (=1)
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      draw();
      // SDL_GL_SwapWindow(window);

      // deprecated for performance reasons...
      // glBegin(GL_QUADS);
      // glColor3f(1.0f, 0.0f, 0.0f);
      // glVertex2f(-0.5f, -0.5f);
      // glVertex2f( 0.5f, -0.5f);
      // glVertex2f( 0.5f,  0.5f);
      // glVertex2f(-0.5f,  0.5f);
      // glEnd();

      // Clear our buffer with a black background
      // This is the same as :
      //       SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
      //       SDL_RenderClear(&renderer);
      //
      // glClearColor(0.0, 0.0, 0.0, 1.0);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // SDL_GL_SwapWindow(window);
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Utility Functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* filetobuf(const char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}