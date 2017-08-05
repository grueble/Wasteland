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
//- Rectangle
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