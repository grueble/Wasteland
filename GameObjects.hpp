#ifndef _GAME_OBJECTS_HPP_
#define _GAME_OBJECTS_HPP_

#include <PhysManager.hpp>

class GameObj
{
public:
   GameObj();

   ~GameObj();

protected:

};

class Platform : GameObj
{
public:
   Platform();

   ~Platform();

protected:
   AABB m_bound_box;

   SceneNode* m_scene_node;

   std::vector<PhysObj> m_hitboxes; // construct hitboxes in place here!
};

// class Player : GameObj
// {
// public:

// protected:
// };

#endif

#ifndef _SCENE_OBJECTS_HPP_
#define _SCENE_OBJECTS_HPP_

#include <list>
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
class PhysObj // hitbox class
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Box : public PhysObj // axis-aligned bounding box
{
public:

protected:

};

class Circle : public PhysObj // circle collision
{
collision:
   Circle();

   ~Circle();

   // calculate collsion with distance function (between 2 circles)? 
   // axis aligned rectanglur bounding boxes
   void render(glm::mat4& projection, glm::mat4& view);

protected:
   // circle_data_t m_coord_data;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - SceneNode & children
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SceneNode // position data, pointer to render obj., pointer to phys obj.
{
public:
   SceneNode();

   ~SceneNode();

   virtual void update(); // updates world xform

protected:
   /* 
    * OK, so the question is: how to relate scene graph, collisions, physics, etc. ?
    * Gonna need a full physics engine implementation, that should be first.
    * From there, decide if the phys can go into the scene graph (via bound boxes)
    * or if a separate structure is needed (i.e. scene graph for positions / render & 
    * quadtree / rtree for phys w bounding boxes (use SAT)). Either way, SceneGraph nodes
    * will need to be updated by the Phys Engine as they are used to draw. I'm thinking
    * that I want to make different node types for geom and phys, with diff. update calls. 
    * Perhaps combine with visibility culling to only push draw objects if the screen BB
    * intersects to SceneNode BB? probably want to tie physics engine saved position or BB 
    * with the version kept by the SceneGraph... 
    * NOTE: quadtree might be clutch for collision, as in generate a screen quad tree every frame,
    *       and then use it to check for collisions with all on screen phys objects?
    *             ~DO FRAMES~
    * finally - does a SceneNode store a PhysObj or visa versa? can we do both? i think I want
    *           to have physics happening first, then update the Scene, so SceneNode xforms need
    *           to be updated by the Physics Engine (specifically collision resolution)
    * alsoooooo, (if possible) Physics Obj should be in a quadtree or rtree that is populated 
    * every frame and occurs off-screen if necessary, allowing for objects to be inserted or 
    * removed based on player input / actions
    */

   Mesh* m_mesh; // all render data & functions // should this be here or in a RenderNode child?

   glm::mat4 m_local_xform; // translation * rotation * scale
   glm::mat4 m_world_xform; // model matrix for vert shader

   std::list<SceneNode> m_children;
};

class Mesh // class to store all render data, probably a texture mesh
{
public:

protected:
};

// OK, diff. types of node: 

// SceneObj : each SceneObj has a hitbox and vertex_data, 
//            render_data. can be vehicle, projectile, etc.

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

#include "GameObjects.hpp"
#include <iostream>

// PhysObj::PhysObj()
// {

// }

PhysObj::PhysObj(GLuint shader_program_id, GLuint mvp_id, glm::mat4 model) :
   m_shader_program_id(shader_program_id),
   m_mvp_id(mvp_id),
   m_model(model)
{

}

PhysObj::~PhysObj()
{
   glDeleteProgram(m_shader_program_id);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Triangle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Triangle::Triangle()
// {
//    // not sure if I want default construction...
// }

Triangle::Triangle(GLuint shader_program_id, GLuint mvp_id, glm::mat4 model, tri_data_t data) :
                   // point2_t a, point2_t b, point2_t c) : 
   PhysObj(shader_program_id, mvp_id, model),
   m_coord_data(data)
{
   glGenBuffers(2, m_vbo); // allocate + assign two VBOs to our handle

   // bind our 1st VBO as the active buffer for storing vertex attr. data (coordinates)
   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
   // copy the vertex data from our triangle data struct to the active buffer
   // glBufferData(GL_ARRAY_BUFFER, sizeof(m_coord_data), 
                // m_coord_data, GL_STATIC_DRAW);

   // bind our 2nd VBO as the active buffer for storing vertex attr. data (colors)
   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
   // copy the vertex data from our color data struct to the active buffer
   // glBufferData(GL_ARRAY_BUFFER, sizeof(g1_color_data), 
                // g1_color_data, GL_STATIC_DRAW);
}

Triangle::~Triangle()
{
   // may want to use this to clean up vao / vbo....
   glDeleteBuffers(2, m_vbo);
}

void Triangle::render(glm::mat4& projection, glm::mat4& view) // maybe try passing in the vertex buffer object in the future
{
   // printf("{ %f, %f, %f, %f, %f, %f }", 
   //    m_coord_data[0], m_coord_data[1], m_coord_data[2], 
   //    m_coord_data[3], m_coord_data[4], m_coord_data[5]);

   /* one future possibility: move ^ constructor code back to its original location (below)
    *    create vbo array in place instead of storing the array as part of the PhysObj
    *    store coorinates in a struct, use them to create glm::mat4 model
    *    AND link mvp ID?!? */ // gonna do this in Rectangle

    glm::mat4 mvp = projection * view * m_model; // m_model; // generate this PhysObj's current MVP

   static const GLfloat g_vertex_buffer_data[3][2] = {
      { m_coord_data.m_a.m_x, m_coord_data.m_a.m_y },
      { m_coord_data.m_b.m_x, m_coord_data.m_b.m_y },
      { m_coord_data.m_c.m_x, m_coord_data.m_c.m_y }
   };

   /* create two Vertex Buffer Objects to give to OpenGL */ 
   // GLuint vbo[2]; // create Vertex Buffer Object handle

   // bind our 1st VBO as the active buffer for reading vertex attr. data (coordinates) 
   glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
   // copy the vertex data from our triangle data struct to the active buffer
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), 
                g_vertex_buffer_data, GL_STATIC_DRAW);
   // specify the format of the data contained @ vertex attr. index 0 (2 floats/vertex)
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(0); // enable attr. @ index 0 (triangle coordinates) for use

   // bind our 2nd VBO as the active buffer for reading vertex attr. data (colors)
   glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
   // copy the vertex data from our color data struct to the active buffer
   glBufferData(GL_ARRAY_BUFFER, sizeof(g1_color_data), 
                g1_color_data, GL_STATIC_DRAW);
   // specify the format of the data contained @ vertex attr. index 1 (3 floats/vertex)
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(1); // enable attr. @ index 1 (colors) for use

   glUseProgram(m_shader_program_id); // load shader into the rendering pipeline

   glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &mvp[0][0]);

   glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting @ 0

   glUseProgram(0);
   glDisableVertexAttribArray(0); // disable attr. @ index 0 for use 
   glDisableVertexAttribArray(1); // disable attr. @ index 1 for use 
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//- Quad
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Quad::Quad(GLuint shader_program_id, GLuint mvp_id, glm::mat4 model, quad_data_t data) :
   PhysObj(shader_program_id, mvp_id, model),
   m_coord_data(data) // NOTE: passing order of point parameters matters!
{
   glGenBuffers(2, m_vbo);
}

Quad::~Quad()
{
   glDeleteBuffers(2, m_vbo);
}

void Quad::render(glm::mat4& projection, glm::mat4& view)
{
   glm::mat4 mvp = projection * view * m_model;

   static const GLfloat g_vertex_buffer_data[6][2] = {
      // 1st tri
      { m_coord_data.m_a.m_x, m_coord_data.m_a.m_y },
      { m_coord_data.m_b.m_x, m_coord_data.m_b.m_y },
      { m_coord_data.m_c.m_x, m_coord_data.m_c.m_y },
      // 2nd tri
      { m_coord_data.m_b.m_x, m_coord_data.m_b.m_y },
      { m_coord_data.m_c.m_x, m_coord_data.m_c.m_y },
      { m_coord_data.m_d.m_x, m_coord_data.m_d.m_y }
   };

   glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), 
                g_vertex_buffer_data, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(0);

   // glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_color_data), 
   //              triangle1_color_data, GL_STATIC_DRAW);
   // glVertexAttribPointer(1, 6, GL_FLOAT, GL_FALSE, 0, (void*)0);
   // glEnableVertexAttribArray(1);

   glUseProgram(m_shader_program_id);

   glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &mvp[0][0]);

   glDrawArrays(GL_TRIANGLES, 0, 6);

   glUseProgram(0);
   glDisableVertexAttribArray(0);
   // glDisableVertexAttribArray(1);
   glDeleteBuffers(2, m_vbo);
}