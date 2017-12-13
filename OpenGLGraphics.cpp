#include "OpenGLGraphics.hpp"

// debug
#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
   GLenum glErr;
   int retCode = 0;

   glErr = glGetError();
   if (glErr != GL_NO_ERROR)
   {
      printf("glError in file %s @ line %d: %d\n", file, line, glErr);
      retCode = 1;
   }

   return retCode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - OpenGLRenderer
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void OpenGLRenderer::init()
{
   shaders_.push_back(loadShaders("shaders/texture.vert", "shaders/texture.frag"));
   // textures_.push_back(glGetUniformLocation(shaders_.at(0), "ourTexture")); // possible vector of texture IDs
   // NOTE: texture ID's may want to be associated with the Mesh
}

void OpenGLRenderer::close()
{
   std::vector<GLuint>::iterator shader_it;
   for (shader_it = shaders_.begin(); shader_it != shaders_.end(); ++shader_it)
   {
      glDeleteProgram(*shader_it);
   }

   std::vector<GLuint>::iterator texture_it;
   for (texture_it = textures_.begin(); texture_it != textures_.end(); ++texture_it)
   {
      glDeleteTextures(1, &(*texture_it));
   }
}

void OpenGLRenderer::attachMesh(OpenGLMesh& mesh, GLuint shader_index, 
                                std::vector<float> vertices, std::vector<unsigned int> indices)
{
   glGenVertexArrays(1, &mesh.vao_);
   glBindVertexArray(mesh.vao_);

   glGenBuffers(1, &mesh.vbo_);
   glGenBuffers(1, &mesh.ebo_);

   mesh.shader_id_ = shaders_.at(shader_index);
   mesh.mvp_id_ = glGetUniformLocation(mesh.shader_id_, "MVP");
   // mesh.texture_id_ = glGetUniformLocation(mesh.shader_id_, "ourTexture");

   // this functionality could be moved to a private func that takes an enum signifiying geometry
   mesh.vertices_ = vertices; // copy construct
   mesh.indices_ = indices;

   const float* g_vertices = &(mesh.vertices_[0]);
   
   const unsigned int* g_indices = &(mesh.indices_[0]);

   glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_);
   glBufferData(GL_ARRAY_BUFFER, 
                mesh.vertices_.size() * sizeof(*g_vertices), 
                g_vertices, 
                GL_STATIC_DRAW);
   
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0); // position attrib.
   
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1); // color attrib.
   
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
   glEnableVertexAttribArray(2); // uv coord. attrib.

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo_);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices_.size() * sizeof(*g_indices), g_indices, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   printOpenGLError();

   // return meshes_.size() - 1; // invariant: no deletion from meshes_
}

int OpenGLRenderer::loadTexture(const char* asset_fpath)
{
   GLuint texture;
   glGenTextures(1, &texture);

   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   SDL_Surface* loaded_img = IMG_Load(asset_fpath);
   if (loaded_img == NULL) {
      printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
   }
   else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loaded_img->w, loaded_img->h,
                   0, GL_BGRA, GL_UNSIGNED_BYTE, loaded_img->pixels);
      // NOTE: GL_BGRA is currently distorting colors
      printOpenGLError(); // glGetError();
      glGenerateMipmap(GL_TEXTURE_2D);
      SDL_FreeSurface(loaded_img);
   }
   glBindTexture(GL_TEXTURE_2D, 0);

   textures_.push_back(texture);

   return textures_.size() - 1;
}

void OpenGLRenderer::render(OpenGLMesh& mesh, int texture_index, glm::mat4 mvp)
{
   if (texture_index >= textures_.size())
   {
      printf("Attempting to render an unloaded texture @ %i!\n", texture_index);
      return;
   }

   if (mesh.vertices_.size() > 0 && mesh.indices_.size() > 0) 
   {
      glBindVertexArray(mesh.vao_);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textures_.at(texture_index));
      
      glUseProgram(mesh.shader_id_);
      glUniformMatrix4fv(mesh.mvp_id_, 1, GL_FALSE, &mvp[0][0]);
      // glUniform1i(mesh.texture_id_, 0);

      glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
      
      glUseProgram(0);

      glBindVertexArray(0);
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// - OpenGLRenderer Utility Functions
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GLuint OpenGLRenderer::loadShaders(const char* vert_fpath, const char* frag_fpath)
{
   // create empty handles for our shaders
   GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

   // read our shaders into the appropriate buffers
   GLchar* vertex_src = filetobuf(vert_fpath);
   GLchar* fragment_src = filetobuf(frag_fpath);

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

char* OpenGLRenderer::filetobuf(const char *file)
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

std::vector<float> OpenGLRenderer::getVerticesAABB(float x_halfwidth, float y_halfwidth)
{
   std::vector<float> vertices = {
      // position                            // color           // uv coords
      -(x_halfwidth),   y_halfwidth,  0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
        x_halfwidth,    y_halfwidth,  0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
        x_halfwidth,  -(y_halfwidth), 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      -(x_halfwidth), -(y_halfwidth), 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   return vertices;
}

std::vector<float> OpenGLRenderer::getVerticesCircle(float r)
{
   std::vector<float> vertices = {
      // position        // color           // uv coords
      -(r),   r,  0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
        r,    r,  0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top-right
        r,  -(r), 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // bottom-right
      -(r), -(r), 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f   // bottom-left
   };

   return vertices;
}

std::vector<float> OpenGLRenderer::getVerticesTri(float x_halfwidth, float y_halfwidth)
{
   std::vector<float> vertices = {
      // position                              // color           // uv coords
      0.0f,            2 * y_halfwidth, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // top-left
      0.0f,            0.0f,            0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // bottom-left
      2 * x_halfwidth, 0.0f,            0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f   // bottom-right
   };

   return vertices;
}

std::vector<unsigned int> OpenGLRenderer::getIndicesAABB()
{
   std::vector<unsigned int> indices = {
      0, 1, 2, // first triangle
      2, 0, 3  // second triangle
   };

   return indices;
}

std::vector<unsigned int> OpenGLRenderer::getIndicesTri()
{
   std::vector<unsigned int> indices = {
      0, 1, 2
   };

   return indices;
}