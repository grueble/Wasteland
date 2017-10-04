#ifndef _RENDER_OBJECTS_HPP_
#define _RENDER_OBJECTS_HPP_

#include <SDL.h>
#include <SDL_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <GL/gl3w.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// utility functions
GLuint loadShaders(const char* vertex_fpath, const char* fragment_fpath);
char* filetobuf(const char *file);

class RenderObj
{
public:
   RenderObj(GLuint shader_program_id, 
             GLuint mvp_id, 
             GLuint texture_id, 
             std::vector<float> vertices);
             // std::vector<float> position,
             // std::vector<float> color,
             // std::vector<float> tex_coords);

   ~RenderObj();

   virtual void render(glm::mat4 view) = 0;

   void loadTexture(const char* fpath);

protected:
   GLuint m_shader_program_id;
   GLuint m_mvp_id;
   GLuint m_texture_id;

   // std::vector<float> m_position;
   // std::vector<float> m_color;
   // std::vector<float> m_tex_coords;
   std::vector<float> m_vertices; 
   GLuint m_vbo;
   GLuint m_ebo;

   GLuint m_texture;
};

class RenderTri : public RenderObj
{
public:
   RenderTri(GLuint shader_program_id, 
             GLuint mvp_id, 
             GLuint texture_id, 
             std::vector<float> vertices);
             // std::vector<float> position,
             // std::vector<float> color,
             // std::vector<float> tex_coords);

   ~RenderTri();

   void render(glm::mat4 view);
};

class RenderQuad : public RenderObj
{
public:
   RenderQuad(GLuint shader_program_id, 
              GLuint mvp_id, 
              GLuint texture_id, 
              std::vector<float> vertices);
              // std::vector<float> position,
              // std::vector<float> color,
              // std::vector<float> tex_coords;

   ~RenderQuad();

   void render(glm::mat4 view);
};

#endif