#include "SceneManager.hpp"

Scene::Scene()
{

}

Scene::~Scene()
{
   std::vector<PhysObj*>::iterator it;
   for (it = scene_objs.begin(); it != scene_objs.end(); ++it)
   {
      // destroy any scene objects, calls glDeleteProgram
      delete(*it);
   }
}

void Scene::loadScene()
{
   GLuint tri_shader_prog_id = loadShaders("shaders/triangle.vert", 
                                           "shaders/triangle.frag");

   GLuint tri_mvp_id = glGetUniformLocation(tri_shader_prog_id, "MVP");

   m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
   // glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // ortho

   // this is camera frame code, will need to be input controlled
   glm::vec3 eye_position = glm::vec3(4.0f, 3.0f, 3.0f);
   glm::vec3 look_at_point = glm::vec3(0.0f, 0.0f, 0.0f);
   glm::vec3 up_dir = glm::vec3(0.0f, 1.0f, 0.0f);
   m_view = glm::lookAt(eye_position, look_at_point, up_dir);

   /* NOTE: model @ origin
    * NOTE: **screen** coords are always of the form: 
    * -> ( [-1.0f, 1.0f], [-1.0f, 1.0f] ) */
   scene_objs.push_back(new Triangle(
         tri_shader_prog_id, tri_mvp_id, glm::mat4(1.0f),
         (point2_t){-1.0f, -1.0f}, (point2_t){1.0f, -1.0f}, (point2_t){0.0f, 1.0f}));

   // GLuint quad_shader_prog_id = loadShaders("shaders/quad.vert", 
   //                                          "shaders/quad.frag");

   // GLuint mvp_id




}

void Scene::drawScene()
{
   std::vector<PhysObj*>::iterator it;
   for (it = scene_objs.begin(); it != scene_objs.end(); ++it)
   {
      (*it)->render(m_projection, m_view);
   }
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