#ifndef _GAME_OBJECTS_HPP_
#define _GAME_OBJECTS_HPP_

#include <vector>
#include <utility>

#include <GL/gl3w.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct point2_s point2_t;
typedef struct tri_data_s tri_data_t;
typedef struct quad_data_s quad_data_t;

// NOTE: no Z / depth, use vert. shader to compute depth
struct point2_s
{
   // point2_s(GLfloat x, GLfloat y) :
   //    m_x(x),
   //    m_y(y)
   // { }

   GLfloat m_x, m_y;
};

// typedef struct point3_s point3_t;
// typedef struct point3_s vector3_t;

// struct point3_s
// {
//    point2_s(GLfloat x, GLfloat y, GLfloat z) :
//       m_x(x),
//       m_y(y),
//       m_z(z)
//    { }

//    GLfloat m_x, m_y, m_z;
// };

struct tri_data_s
{
   // tri_data_s(point2_t a, point2_t b, point2_t c) :
   //    m_a(a),
   //    m_b(b),
   //    m_c(c)
   // { }  

   point2_t m_a, m_b, m_c;
};

struct quad_data_s
{
   point2_t m_a, m_b, m_c, m_d;
};

/* NOTE: these (^) are here BC any arrays created for 
 * use in vao/vbo need to persist / get new'd */
static const GLfloat g1_color_data[] = {
   1.0f, 0.0f, 0.0f, // red
   0.0f, 1.0f, 0.0f, // green
   0.0f, 0.0f, 1.0f, // blue
   // 1.0f, 1.0f, 1.0f, // white
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - PhysObj & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class PhysObj 
{
public:
   PhysObj(GLuint shader_program_id, GLuint mvp_id, glm::mat4 model);

   ~PhysObj();

   virtual void render(glm::mat4& projection, glm::mat4& view) = 0;

protected:
   GLuint m_vbo[2]; // coord & color buffers
   GLuint m_shader_program_id;
   GLuint m_mvp_id;
   GLuint m_texture_id;
   glm::mat4 m_model;
};

class Triangle : public PhysObj
{
public:
   Triangle(GLuint shader_program_id, 
            GLuint mvp_id, 
            glm::mat4 model,
            tri_data_t data);
            // point2_t a, point2_t b, point2_t c);

   ~Triangle();

   void render(glm::mat4& projection, glm::mat4& view);

protected:
   // GLfloat m_coord_data[6];
   tri_data_t m_coord_data;
};

class Quad : public PhysObj
{
public:
   Quad(GLuint shader_program_id, 
        GLuint mvp_id, 
        glm::mat4 model,
        quad_data_t data);
        // point2_t a, point2_t b, point2_t c, point2_t d);

   ~Quad();

   void render(glm::mat4& projection, glm::mat4& view);

protected:
   // GLfloat m_coord_data[12];
   quad_data_t m_coord_data;
};

/* ANOTHER COOL IDEA
 * for projectile class PhysObj, calculate full trajectory when fired,
 * but use a timed draw to show movement - allows for frame speed control + + */

/* OK, so I will eventually have to decide between storing position data
 * as a GLfloat[] or struct (i.e. tri_data_t = 3x point3_t). This will be 
 * decided by efficiency and whether memory allocation or loop speed is my
 * limiting variable */

/* TRACKS
 * create as a trigonometric function, start vehicle at origin and render 
 * track (using a texture) and calculating all values of y using screen x values
 */

#endif