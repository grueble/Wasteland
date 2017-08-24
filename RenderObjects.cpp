#include "RenderObjects.hpp"

RenderObj::RenderObj(GLuint shader_program_id, 
                     GLuint mvp_id, 
                     GLuint texture_id, 
                     // std::vector<float> position,
                     // std::vector<float> color,
                     // std::vector<float> tex_coords) :
                     std::vector<float> vertices) :
   m_shader_program_id(shader_program_id),
   m_mvp_id(mvp_id),
   m_texture_id(texture_id),
   // m_position(position),
   // m_color(color),
   // m_tex_coords(tex_coords)
   m_vertices(vertices)
{
   glGenBuffers(1, &m_vbo);
   // glGenBuffers(3, m_vbo);
   glGenBuffers(1, &m_ebo);
   glGenTextures(1, &m_texture);
}

RenderObj::~RenderObj()
{
   glDeleteBuffers(1, &m_vbo);
   // glDeleteBuffers(3, m_vbo);
   glDeleteBuffers(1, &m_ebo);
   glDeleteProgram(m_shader_program_id);
   glDeleteTextures(1, &m_texture);
}

void RenderObj::loadTexture(const char* fpath)
{
   glBindTexture(GL_TEXTURE_2D, m_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   SDL_Surface* loaded_img = IMG_Load(fpath);
   if (loaded_img == NULL) {
      printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
   }
   else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loaded_img->w, loaded_img->h,
                   0, GL_RGB, GL_UNSIGNED_BYTE, loaded_img->pixels);
      glGetError();
      glGenerateMipmap(GL_TEXTURE_2D);
      SDL_FreeSurface(loaded_img);
   }
}

RenderTri::RenderTri(GLuint shader_program_id, 
                     GLuint mvp_id, 
                     GLuint texture_id, 
                     // std::vector<float> position,
                     // std::vector<float> color,
                     // std::vector<float> tex_coords) :
                     std::vector<float> vertices) :
   RenderObj(shader_program_id, mvp_id, texture_id, vertices)
   // RenderObj(shader_program_id, mvp_id, texture_id, position, color, tex_coords)
{
   
}

RenderTri::~RenderTri()
{

}

void RenderTri::render(glm::mat4 mvp)
{
   // glm::mat4 mvp = m_projection * view * m_model;

   // static const float* pos_vertices = &m_position[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vertices), pos_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(0); 

   // static const float* color_vertices = &m_color[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(color_vertices), color_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(1);

   // static const float* tex_vertices = &m_tex_coords[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(tex_vertices), tex_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(2);

   static const float* g_vertices = &m_vertices[0];

   static const unsigned int g_indices[] = {
      0, 1, 2
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
   glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(*g_vertices), g_vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0); // position attrib.

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1); // color attrib.

   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
   glEnableVertexAttribArray(2); // uv coord. attrib.

   GLuint texture;
   glGenTextures(1, &texture);

   glUseProgram(m_shader_program_id);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, m_texture);
   glUniform1i(m_texture_id, 0);

   glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &mvp[0][0]);

   glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

   glUseProgram(0);
   glDisableVertexAttribArray(0); 
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);
}

RenderQuad::RenderQuad(GLuint shader_program_id, 
                       GLuint mvp_id, 
                       GLuint texture_id, 
                       // std::vector<float> position,
                       // std::vector<float> color,
                       // std::vector<float> tex_coords) :
                       std::vector<float> vertices) :
   RenderObj(shader_program_id, mvp_id, texture_id, vertices)
   // RenderObj(shader_program_id, mvp_id, texture_id, position, color, tex_coords)
{

}

RenderQuad::~RenderQuad()
{

}

void RenderQuad::render(glm::mat4 mvp)
{
   // glm::mat4 mvp = m_projection * view * model;

   // static const float* pos_vertices = &m_position[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vertices), pos_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(0); 

   // static const float* color_vertices = &m_color[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(color_vertices), color_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(1);

   // static const float* tex_vertices = &m_tex_coords[0];

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(tex_vertices), tex_vertices, GL_STATIC_DRAW);
   // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(2);

   static const float* g_vertices = &m_vertices[0];

   static const unsigned int g_indices[] = {
      0, 1, 3, // first tri
      1, 2, 3  // second tri
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
   glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(*g_vertices), g_vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
   glEnableVertexAttribArray(2);

   GLuint texture;
   glGenTextures(1, &texture);

   glUseProgram(m_shader_program_id);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, m_texture);
   glUniform1i(m_texture_id, 0);

   glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &mvp[0][0]);

   glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

   glUseProgram(0);
   glDisableVertexAttribArray(0); 
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Utility Functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GLuint loadShaders(const char* vertex_fpath, const char* fragment_fpath)
{
   // create empty handles for our shaders
   GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

   // read our shaders into the appropriate buffers
   GLchar* vertex_src = filetobuf(vertex_fpath);
   GLchar* fragment_src = filetobuf(fragment_fpath);

   GLint result = GL_FALSE;
   int log_length;

   // send the vertex shader src code to GL; compile; check for (and handle?) errors
   glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_src, 0);
   glCompileShader(vertex_shader);
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) 
   {
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
      if (log_length > 0)
      {
         GLchar vertex_shader_info_log[log_length + 1];
         glGetShaderInfoLog(vertex_shader, log_length, NULL, &vertex_shader_info_log[0]);
         printf("Vertex shader compilation failure!\n%s\n", vertex_shader_info_log);
      }
   }

   // send the fragment shader src code to GL; compile; check for (and handle?) errors
   glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_src, 0);
   glCompileShader(fragment_shader);
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) 
   {
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
      if (log_length > 0)
      {
         GLchar fragment_shader_info_log[log_length + 1];
         glGetShaderInfoLog(fragment_shader, log_length, NULL, &fragment_shader_info_log[0]);
         printf("Fragment shader compilation failure!\n%s\n", fragment_shader_info_log);
      }
   }

   // create an empty handle for our linked shader program
   GLuint shader_program = glCreateProgram();

   // attach our shaders to our program
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);

   /* bind attr. @ index 0 (coordinates) to in_Position and attr. @ index 1 (color) to in_Color
    * -> NOTE: attr. locations must be setup before calling glLinkProgram */
   // glBindAttribLocation(shader_program, 0, "in_Position"); // layout(location = 0)  in .vert
   // glBindAttribLocation(shader_program, 1, "in_Color"); // layout(location = 1)  in .vert
   glLinkProgram(shader_program); // link our program

   // check for proper linkage between the vertex and fragment shaders (and other errors)
   glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
   if (result == GL_FALSE)
   {
      glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
      if (log_length > 0)
      {
         GLchar shader_program_info_log[log_length + 1];
         glGetShaderInfoLog(shader_program, log_length, NULL, &shader_program_info_log[0]);
         printf("Shader program linking failure!\n%s\n", shader_program_info_log);
      }
   }

   glDetachShader(shader_program, vertex_shader);
   glDetachShader(shader_program, fragment_shader);
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   free(vertex_src);
   free(fragment_src); 

   return shader_program;
}

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