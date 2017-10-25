#ifndef _OPENGL_GRAPHICS_HPP_
#define _OPENGL_GRAPHICS_HPP_

#include <SDL.h>
#include <SDL_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <GL/gl3w.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Physics.hpp"

typedef struct OpenGLMesh Mesh_t;
typedef struct OpenGLRenderer Renderer_t;

/* Well this is getting complicated...
 *
 * -> OpenGLRenderer now owns & manages all of the rendering
 *    objects & properties
 *
 * -> Currently, meshes_ must be refactored into a custom
 *    data type that supports deletion without messing up
 *    every object's index in the list
 * ---> Eventually, this may require the implementation of some
 *      kind of insertion or deletion list on a separate thread
 *
 * -> May want to split mesh object into a 'RenderObj' (vao,
 *    vertices, etc.) & a 'Texture' (GLuint texture &
 *    possibly more specific properties, i.e. multiple binds)
 *    at some future point
 *
 * -> OpenGLMesh can inherit an interface if methods become a 
 *    requirement - but it will also need numerous getters and 
 *    setters (and even bind functions for the global GL state)
 *
 * -> Would be ideal to decouple this module from the physics 
 *    module at some future point. An enum could be used to 
 *    differentiate between mesh types at initialization
 *    (which really only affects setting of vertices/indices)
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - OpenGLMesh
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct OpenGLMesh
{
   GLuint vao_;
   GLuint vbo_;
   GLuint ebo_;
   GLuint texture_; // in some ways, this is the true texture ID...
   std::vector<float> vertices_;
   std::vector<unsigned int> indices_;
   GLuint shader_id_;
   GLuint mvp_id_;
   // GLuint texture_id_; // possible to use multiple textures...
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - OpenGLRenderer
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class OpenGLRenderer
{
public:
   init();

   close();

   int createMesh(const char* asset_fpath, 
                  GLuint shader_index, 
                  std::vector<float> vertices, 
                  std::vector<unsigned int> indices);

   void render(int mesh_index, glm::mat4 mvp);

   void printMesh(int mesh_index);

private: 
   GLuint loadShaders(const char* vert_fpath, const char* frag_fpath);

   char* filetobuf(const char *file);

   std::vector<GLuint> shaders_;
   std::vector<OpenGLMesh> meshes_;
};

#endif