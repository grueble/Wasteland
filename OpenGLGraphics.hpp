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

namespace glGraphics_
{
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //
   // - OpenGLMesh
   //
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   struct OpenGLMesh
   {
      GLuint vao;
      GLuint vbo;
      GLuint ebo;
      std::vector<float> vertices;
      std::vector<unsigned int> indices;
      GLuint shader_id;
      GLuint mvp_id;
      // GLuint texture_id; // possible to use multiple textures...
   };
   
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //
   // - OpenGLRenderer
   //
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   class OpenGLRenderer
   {
   public:
      void init();
   
      void close();
   
      void attachMesh(OpenGLMesh& mesh,
                     GLuint shader_index, 
                     std::vector<float> vertices, 
                     std::vector<unsigned int> indices);
   
      int loadTexture(const char* asset_fpath);
   
      void render(OpenGLMesh& mesh, int texture_index, glm::mat4 mvp);
   
      std::vector<float> getVerticesAABB(float x_halfwidth, float y_halfwidth);
      std::vector<float> getVerticesCircle(float r);
      std::vector<float> getVerticesTri(float x_halfwidth, float y_halfwidth);
   
      std::vector<unsigned int> getIndicesAABB();
      std::vector<unsigned int> getIndicesTri();
   
   private: 
      GLuint loadShaders(const char* vert_fpath, const char* frag_fpath);
   
      char* filetobuf(const char *file);
   
      std::vector<GLuint> shaders_;
      std::vector<GLuint> textures_;
   };
};

// this should be typedef's in a controller class or include header
typedef struct glGraphics_::OpenGLRenderer Renderer_t;
typedef struct glGraphics_::OpenGLMesh Mesh_t;
// typedef struct glGraphics_::OpenGLTexture Texture_t;

#endif